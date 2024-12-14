#include <iostream>
#include "particle.hpp"
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
using namespace std;

Coordinate Particle::globalBest=Coordinate(0,0);

#define N_PARTICLES 100
#define LOWER_BOUND -4.5
#define UPPER_BOUND 4.5
#define N_ITERATIONS 15

double clamp(double val) {
  if (val < -1.0) return -1.0;
  if (val > 1.0) return 1.0;
  return val;
}

enum FunctionType { RASTRIGIN, SPHERE, ROSENBROCK ,BAELE, STRANGE};

FunctionType currentFunction = BAELE;

double rastrigin(Coordinate x) {
    //return the rastrigin function value
    return 20 + pow(x.getX(), 2) + pow(x.getY(), 2) - 10 * (cos(2 * M_PI * x.getX()) + cos(2 * M_PI * x.getY()));
    
}
double strangeFunction(Coordinate x) {
    //return the himmelblau function value
    return pow(pow(x.getX(), 2) + x.getY() - 11,2)+ pow(pow(x.getY(), 2) + x.getX() - 7,2);
    
}

double baele(Coordinate x) {
  //return the beale function value
    // Combina i termini e riduce l'ampiezza
    return (1.5-x.getX()+x.getX()*x.getY())*(1.5-x.getX()+x.getX()*x.getY())+(2.25-x.getX()+x.getX()*x.getY()*x.getY())*(2.25-x.getX()+x.getX()*x.getY()*x.getY())+(2.625-x.getX()+x.getX()*x.getY()*x.getY()*x.getY())*(2.625-x.getX()+x.getX()*x.getY()*x.getY()*x.getY());
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
        case BAELE:
            return baele(x);
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
    for (double x = LOWER_BOUND; x <= UPPER_BOUND; x += 0.1) {
        for (double y = LOWER_BOUND; y <= UPPER_BOUND; y += 0.1) {
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
        double vx = inertia * particles[i].getVelocity().getX() + cognitive * (particles[i].getPersonalBest().getX() - particles[i].getPosition().getX()) + social * (Particle::getGlobalBest().getX() - particles[i].getPosition().getX());
        double vy = inertia * particles[i].getVelocity().getY() + cognitive * (particles[i].getPersonalBest().getY() - particles[i].getPosition().getY()) + social * (Particle::getGlobalBest().getY() - particles[i].getPosition().getY());
        vx = clamp(vx);
        vy = clamp(vy);
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
        if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(Particle::getGlobalBest())) {
            Particle::updateGlobalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
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
            Particle::updateGlobalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }
        else{
            //update the global best if the particle is better
            if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(Particle::getGlobalBest())) {
                Particle::updateGlobalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
            }
        }
        //print the particle position
        //cout << "Particle " << i << " position: " << particles[i].getPosition().getX() << " " << particles[i].getPosition().getY() << endl;
    }
    float inertia = 0.5;
    float cognitive = 0.5;
    float social = 1.5;
    saveGridData();
    for(int i = 0; i < N_ITERATIONS; i++){
        updates(inertia, cognitive, social, particles);
        saveParticleData(particles, i);
    }
    cout << "Final Global Best: (" 
     << Particle::getGlobalBest().getX() << ", " 
     << Particle::getGlobalBest().getY() << ") Value: " 
     << evaluateFunction(Particle::getGlobalBest()) << endl;
    /*for (int i = 0; i < N_PARTICLES; i++) {
        //print the particle position
        cout << "Particle " << i << " position: " << particles[i].getPosition().getX() << " " << particles[i].getPosition().getY() << endl;
    }*/
   

    
    return 0;
}