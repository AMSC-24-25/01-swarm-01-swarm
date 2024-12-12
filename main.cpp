#include <iostream>
#include "particle.hpp"
#include <vector>
#include <random>
#include <cmath>

#define N_PARTICLES 5
#define LOWER_BOUND -100
#define UPPER_BOUND 100

/*double clamp(double val) {
  if (val < LOWER_BOUND) return LOWER_BOUND;
  if (val > UPPER_BOUND) return UPPER_BOUND;
  return val;
}*/

using namespace std;
Coordinate Particle::globalBest;

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


    return 0;
}