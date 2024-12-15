#include <iostream>
#include "particle.hpp"
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <omp.h>
#include <ctime>
#include <sys/time.h>


using namespace std;

Coordinate Particle::globalBest=Coordinate(0,0);

int N_PARTICLES, N_ITERATIONS; 
float LOWER_BOUND, UPPER_BOUND;

double clamp(double val) {
  if (val < -20) return -20;
  if (val > 20) return 20;
  return val;
}

enum FunctionType { RASTRIGIN, SPHERE, ROSENBROCK ,BAELE, STRANGE};

FunctionType currentFunction = BAELE;
double eggholder(Coordinate x) {
    //return the eggholder function value
    return -(x.getY() + 47) * sin(sqrt(abs(x.getY() + x.getX() / 2 + 47))) - x.getX() * sin(sqrt(abs(x.getX() - (x.getY() + 47))));
}
double simionescu(Coordinate x) {
    //return the simionescu function value
    return 0.5 + (pow(sin(sqrt(pow(x.getX(), 2) + pow(x.getY(), 2))), 2) - 0.5 / (1 + 0.001 * (pow(x.getX(), 2) + pow(x.getY(), 2))));
}
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
            return eggholder(x);
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
    for (double x = -2; x <= 2; x += 0.1) {
        for (double y = -2; y <= 2; y += 0.1) {
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
    //print out the dimension of particles
    cout << particles.size() << endl;
    
    for (const auto& particle : particles) {
        file << particle.getPosition().getX() << "," << particle.getPosition().getY() << "," << evaluateFunction(particle.getPosition()) << "\n";
    }
    file.close();
}

void setupParticlesParallel(vector<Particle> &particles) {
    random_device rd;   
    mt19937 seed(rd());
    uniform_real_distribution<> randomGenerator(LOWER_BOUND, UPPER_BOUND);
    #pragma omp parallel for schedule(dynamic, 10) num_threads(2)
    for (int i = 0; i < N_PARTICLES; i++) {
        particles[i] = Particle(randomGenerator(seed), randomGenerator(seed), randomGenerator(seed), randomGenerator(seed));
        particles[i].updatePersonalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        //set the global best to the first particle
        if (i == 0) {
            Particle::updateGlobalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }
        #pragma omp critical
        //update the global best if the particle is better
        if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(Particle::getGlobalBest())) {
            Particle::updateGlobalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
            }
        
        }
    
}

void setupParticlesSerial(vector<Particle> &particles) {
    random_device rd;   
    mt19937 seed(rd());
    uniform_real_distribution<> randomGenerator(LOWER_BOUND, UPPER_BOUND);
    
    
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
        
    }
}




void updatesParallel(float inertia, float cognitive, float social, vector<Particle> &particles) {
    #pragma omp parallel for schedule(dynamic, 10) num_threads(2)
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
            
            double z = evaluateFunction(particles[i].getPosition());
            #pragma omp critical
            if (z < evaluateFunction(Particle::getGlobalBest())) {
                Particle::updateGlobalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
            }
        }
}

void updatesSerial(float inertia, float cognitive, float social, vector<Particle> &particles) {
    
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
        double z = evaluateFunction(particles[i].getPosition());
        //update global best
        
        if (z < evaluateFunction(Particle::getGlobalBest()) ) {
            Particle::updateGlobalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }
        
    }

}

int main(int argc, char *argv[]) {

    if (argc < 8){
      cerr << "Usage: " << argv[0] << " N_Particles lowerBound upperBound N_Iterations inertiaW cognitiveW socialW" << endl;
      return 1;
    }
    
    N_PARTICLES = atoi(argv[1]);
    LOWER_BOUND = stof(argv[2]);
    UPPER_BOUND = stof(argv[3]);
    N_ITERATIONS = atoi(argv[4]);
    float inertia = stof(argv[5]);
    float cognitive = stof(argv[6]);
    float social = stof(argv[7]);
    struct timeval t1,t2,t3,t4,t5,t6,t7,t8;
    double etime, etime1,etime2,etime3;
    vector<Particle> particlesParallel(N_PARTICLES);
    
	gettimeofday(&t1, NULL);	
    //setupParticlesParallel(particlesParallel); 
	gettimeofday(&t2, NULL);	
    etime = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_usec - t1.tv_usec) / 1000;
	etime=etime/1000;

    vector<Particle> particlesSerial(N_PARTICLES);
	gettimeofday(&t3, NULL);	
    setupParticlesSerial(particlesSerial); 
	gettimeofday(&t4, NULL);	
    etime1 = (t4.tv_sec - t3.tv_sec) * 1000 + (t4.tv_usec - t3.tv_usec) / 1000;
	etime1 = etime1/1000;
    cout<<"Serial setup time: "<<etime1<<endl;
    //print out the parameter for debugging
    cout << "N_PARTICLES: " << N_PARTICLES << endl;
    cout << "LOWER_BOUND: " << LOWER_BOUND << endl;
    cout << "UPPER_BOUND: " << UPPER_BOUND << endl;
    cout << "N_ITERATIONS: " << N_ITERATIONS << endl;
    cout << "Inertia: " << inertia << endl;
    cout << "Cognitive: " << cognitive << endl;
    cout << "Social: " << social << endl;

   
    saveGridData();

   
   
    gettimeofday(&t7, NULL);	
    for(int i = 0; i < N_ITERATIONS; i++){
        updatesParallel(inertia, cognitive, social, particlesParallel);
        //saveParticleData(particlesParallel, i);
    }
    gettimeofday(&t8, NULL);
    etime3 = (t8.tv_sec - t7.tv_sec) * 1000 + (t8.tv_usec - t7.tv_usec) / 1000;
    etime3 = etime3 / 1000;
    
    gettimeofday(&t5, NULL);
    for(int i = 0; i < N_ITERATIONS; i++){
        updatesSerial(inertia, cognitive, social, particlesSerial);
        //saveParticleData(particlesSerial, i);
        
    }
    gettimeofday(&t6, NULL);
    etime2 = (t6.tv_sec - t5.tv_sec) * 1000 + (t6.tv_usec - t5.tv_usec) / 1000;
    etime2 = etime2 / 1000;
    
    cout << "Final Global Best: (" 
    << Particle::getGlobalBest().getX() << ", " 
    << Particle::getGlobalBest().getY() << ") Value: " 
    << evaluateFunction(Particle::getGlobalBest()) << endl;
    
    cout << "Parallel Execution Time: " <<etime3+etime<< "  milliseconds" << endl;
    cout << "Serial Execution Time: " <<etime1+etime2<< "  milliseconds" << endl;
    
    cout<<" Speedup: "<< (etime1+etime2)/(etime+etime3) << endl;


    //system("python3 animation.py");


    return 0;
}