#include <iostream>
#include "particle.hpp"
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
using namespace std;

Coordinate Particle::globalBest=Coordinate();

#define N_PARTICLES 25
#define LOWER_BOUND -300
#define UPPER_BOUND 300
#define N_ITERATIONS 25

/*double clamp(double val) {
  if (val < LOWER_BOUND) return LOWER_BOUND;
  if (val > UPPER_BOUND) return UPPER_BOUND;
  return val;
}*/

enum FunctionType { RASTRIGIN, SPHERE, ROSENBROCK , STRANGE};

FunctionType currentFunction = SPHERE;

double rastrigin(Coordinate x) {
    //return the rastrigin function value
    return 20 + pow(x.getX(), 2) + pow(x.getY(), 2) - 10 * (cos(2 * M_PI * x.getX()) + cos(2 * M_PI * x.getY()));
    
}

double strangeFunction(Coordinate x) {
    double xVal = x.getX();
    double yVal = x.getY();

    // Termini con valori ridotti
    double term1 = sin(xVal) * cos(yVal) * exp(-abs(sqrt(xVal * xVal + yVal * yVal) / M_PI));
    double term2 = 5 * pow(sin(xVal * yVal), 2);

    // Combina i termini e riduce l'ampiezza
    return -fabs(term1) + term2 - 0.5 * (xVal * xVal + yVal * yVal);
}


double sphere(Coordinate x) {
    return x.getX() * x.getX() + x.getY() * x.getY()+ sqrt(x.getX() * x.getX() + x.getY() * x.getY())*0.5-sqrt(x.getX())*0.5+sqrt(x.getY())*0.5-x.getX()*x.getY();
}

double rosenbrock(Coordinate x) {
    return 100 * pow(x.getY() - pow(x.getX(), 2), 2) + pow(1 - x.getX(), 2);
}

double evaluateFunction(Coordinate x) {
    switch (currentFunction) {
        case RASTRIGIN:
            return rastrigin(x);
        case SPHERE:
            return sphere(x);
        case ROSENBROCK:
            return rosenbrock(x);
        case STRANGE:
            return strangeFunction(x);
        default:
            cerr << "Funzione non riconosciuta!" << endl;
            exit(1);
    }
}





void saveGridData() {
    std::ofstream file("animation/grid.csv");
    if (!file.is_open()) {
        cerr << "Errore nell'aprire il file per la griglia!" << endl;
        return;
    }
    file << "x,y,z\n";  // Intestazione CSV
    for (double x = LOWER_BOUND; x <= UPPER_BOUND; x += 3) {
        for (double y = LOWER_BOUND; y <= UPPER_BOUND; y += 3) {
            double z = evaluateFunction(Coordinate(x, y));
            file << x << "," << y << "," << z << "\n";
        }
    }
    file.close();
}

void saveParticleData(const vector<Particle>& particles, int iteration) {
    std::ofstream file("animation/particles_" + std::to_string(iteration) + ".csv");
    if (!file.is_open()) {
        cerr << "Errore nell'aprire il file per le particelle!" << endl;
        return;
    }
    file << "x,y,z\n";  // Intestazione CSV
    for (const auto& particle : particles) {
        file << particle.getPosition().getX() << "," << particle.getPosition().getY() << "," << evaluateFunction(particle.getPosition()) << "\n";
    }
    file.close();
}






void updates(float inertia, float cognitive, float social, vector<Particle> &particles) {
    for (int i = 0; i < N_PARTICLES; i++) {
        //update velocity
        double vx = inertia * particles[i].getVelocity().getX() + cognitive * (particles[i].getPersonalBest().getX() - particles[i].getPosition().getX()) + social * (particles[i].getGlobalBest().getX() - particles[i].getPosition().getX());
        double vy = inertia * particles[i].getVelocity().getY() + cognitive * (particles[i].getPersonalBest().getY() - particles[i].getPosition().getY()) + social * (particles[i].getGlobalBest().getY() - particles[i].getPosition().getY());
        particles[i].updateVelocity(vx, vy);
        //update position
        double x = particles[i].getPosition().getX() + vx;
        double y = particles[i].getPosition().getY() + vy;
        particles[i].updatePosition(x, y);
        //update personal best
        if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(particles[i].getPersonalBest())) {
            particles[i].updatePersonalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }
        //update global best
        if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(particles[i].getGlobalBest())) {
            particles[i].getGlobalBest().setCoordinate(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }
    }

}

int main() {
    
    random_device rd;   
    mt19937 seed(rd());
    uniform_real_distribution<> randomGenerator(LOWER_BOUND, UPPER_BOUND);
    
    vector<Particle> particles(N_PARTICLES);
    for (int i = 0; i < N_PARTICLES; i++) {
        particles[i] = Particle(randomGenerator(seed), randomGenerator(seed), randomGenerator(seed), randomGenerator(seed));
        particles[i].updatePersonalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        //set the global best to the first particle
        if (i == 0) {
            particles[i].getGlobalBest().setCoordinate(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }
        else{
            //update the global best if the particle is better
            if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(particles[i].getGlobalBest())) {
                particles[i].getGlobalBest().setCoordinate(particles[i].getPosition().getX(), particles[i].getPosition().getY());
            }
        }
        //print the particle position
        //cout << "Particle " << i << " position: " << particles[i].getPosition().getX() << " " << particles[i].getPosition().getY() << endl;
    }
    float inertia = 0.5;
    float cognitive = 0.5;
    float social = 0.5;
    saveGridData();
    for(int i = 0; i < N_ITERATIONS; i++){
        updates(inertia, cognitive, social, particles);
        saveParticleData(particles, i);
    }
    cout << "Global best: " << evaluateFunction(particles[0].getGlobalBest()) <<endl;
    /*for (int i = 0; i < N_PARTICLES; i++) {
        //print the particle position
        cout << "Particle " << i << " position: " << particles[i].getPosition().getX() << " " << particles[i].getPosition().getY() << endl;
    }*/
   

    
    return 0;
}