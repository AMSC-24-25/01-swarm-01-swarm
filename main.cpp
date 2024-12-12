#include <iostream>
#include "particle.hpp"
#include <vector>
#include <random>
#include <cmath>

using namespace std;
Coordinate Particle::globalBest=Coordinate();

#define N_PARTICLES 5
#define LOWER_BOUND -100
#define UPPER_BOUND 100

/*double clamp(double val) {
  if (val < LOWER_BOUND) return LOWER_BOUND;
  if (val > UPPER_BOUND) return UPPER_BOUND;
  return val;
}*/


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
    }

}

int main() {
    
    random_device rd;   
    mt19937 seed(rd());
    uniform_real_distribution<> randomGenerator(LOWER_BOUND, UPPER_BOUND);
    
    vector<Particle> particles(N_PARTICLES);
    for (int i = 0; i < N_PARTICLES; i++) {
        particles[i] = Particle(randomGenerator(seed), randomGenerator(seed), randomGenerator(seed), randomGenerator(seed));
        //print the particle position
        cout << "Particle " << i << " position: " << particles[i].getPosition().getX() << " " << particles[i].getPosition().getY() << endl;
    }
    float inertia = 0.5;
    float cognitive = 0.5;
    float social = 0.5;
    for(int i = 0; i < 100; i++){
        updates(inertia, cognitive, social, particles);
    }
    for (int i = 0; i < N_PARTICLES; i++) {
        //print the particle position
        cout << "Particle " << i << " position: " << particles[i].getPosition().getX() << " " << particles[i].getPosition().getY() << endl;
    }
    return 0;
}