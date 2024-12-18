#include "particle_test.hpp"
#include "functions.hpp"
#include <random>
#include <omp.h>
#include <iostream>
#include <cmath>

using namespace std;


// you have std::clamp that doas that. Anyway, it is fine. 
double clampVel(double val) {
  if (val < -20) return -20;
  if (val > 20) return 20;
  return val;
}
void clampPos(double* val, float lowerBound, float upperBound) {
    if (*val < lowerBound) *val = lowerBound;
    if (*val > upperBound) *val = upperBound;
}

void setupParticlesParallel(vector<Particle>& particles, float lowerBound, float upperBound) {
    random_device rd;
    mt19937 seed(rd());
    uniform_real_distribution<> randomGenerator(lowerBound, upperBound);
    //Error! You are using random distribution wrongly!!! distributions call thake in input the number generated bay an engine! Look at the examples in cppreferefnce
    // Moreover, engines are stateful, so cannot be shared in a multithread environment. You have a data race
    /*
    The reason is that the engine store a value that is updated at any call. SO two treads may ge into confict.
    you should do something like that:

    Generate a few seeds using seed_seq. You need a different seeds to feed the 4 generators for each thread
    You can use the random device to initialize seed_seq a few values (4 bastano)
    auto seq = seed_seq(rd(),rd(),rd(),rd());
    auto num_seeds=numthreads; (numthread e` il numero di thread che volete usare)
    std::vector<std::unit32_t> seeds(num_seeds);
    seq.generate(seeds.begin(),seeds(end));// cosi avete dei seed ben distribuiti per la massima entropia
    #pragma omp parallel // aprite al sezione parallela
    auto rank = omp_num_thread(); // beccate il rank del thread (dovete includere omp.h)
    std::mt19937 engine(seeds(rank)); // one engine per thread
    #pragma omp for schedule(dynamic, 10) num_threads(numthreads)
        for (size_t i = 0; i < particles.size(); i++) {
          particles[i] = Particle(randomGenerator(engine()), randomGenerator(engin()), randomGenerator(engine()), randomGenerator(engine()));

    */
    #pragma omp parallel for schedule(dynamic, 10) num_threads(2)
    for (size_t i = 0; i < particles.size(); i++) {
        particles[i] = Particle(randomGenerator(seed), randomGenerator(seed), randomGenerator(seed), randomGenerator(seed));
        particles[i].updatePersonalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        double vx = clampVel(particles[i].getVelocity().getX());
        double vy = clampVel(particles[i].getVelocity().getY());
        particles[i].updateVelocity(vx, vy);
        #pragma omp critical
        if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(Particle::getGlobalBest())) {
            Particle::updateGlobalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }
    }
    /*#pragma omp critical
    for (size_t i = 0; i < particles.size(); i++) {
        //print out the particles for debugging
        cout << "Particle " << i << " Position: (" << particles[i].getPosition().getX() << ", " << particles[i].getPosition().getY() << ") Velocity: (" << particles[i].getVelocity().getX() << ", " << particles[i].getVelocity().getY() << ") Personal Best: (" << particles[i].getPersonalBest().getX() << ", " << particles[i].getPersonalBest().getY() << ")" << endl;
    }*/
}

void setupParticlesSerial(vector<Particle>& particles, float lowerBound, float upperBound) {
    random_device rd;
    mt19937 seed(rd());
    uniform_real_distribution<> randomGenerator(lowerBound, upperBound);

    for (size_t i = 0; i < particles.size(); i++) {
        particles[i] = Particle(randomGenerator(seed), randomGenerator(seed), randomGenerator(seed), randomGenerator(seed));
        particles[i].updatePersonalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());

        if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(Particle::getGlobalBest())) {
            Particle::updateGlobalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }
    }
}

void updatesParallel(float inertia, float cognitive, float social, vector<Particle>& particles, float lowerBound, float upperBound) {
    #pragma omp parallel for schedule(dynamic, 10) num_threads(2)
    for (size_t i = 0; i < particles.size(); i++) {
        double vx = inertia * particles[i].getVelocity().getX() + cognitive * (particles[i].getPersonalBest().getX() - particles[i].getPosition().getX()) + social * (Particle::getGlobalBest().getX() - particles[i].getPosition().getX());
        double vy = inertia * particles[i].getVelocity().getY() + cognitive * (particles[i].getPersonalBest().getY() - particles[i].getPosition().getY()) + social * (Particle::getGlobalBest().getY() - particles[i].getPosition().getY());
        vx = clampVel(vx);
        vy = clampVel(vy);
        particles[i].updateVelocity(vx, vy);

        double x = particles[i].getPosition().getX() + vx;
        double y = particles[i].getPosition().getY() + vy;
        clampPos(&x, lowerBound, upperBound);
        clampPos(&y, lowerBound, upperBound);
        particles[i].updatePosition(x, y);

        if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(particles[i].getPersonalBest())) {
            particles[i].updatePersonalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }

        #pragma omp critical
        if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(Particle::getGlobalBest())) {
            Particle::updateGlobalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }
    }
}

void updatesSerial(float inertia, float cognitive, float social, vector<Particle>& particles, float lowerBound, float upperBound) {
    for (size_t i = 0; i < particles.size(); i++) {
        double vx = inertia * particles[i].getVelocity().getX() + cognitive * (particles[i].getPersonalBest().getX() - particles[i].getPosition().getX()) + social * (Particle::getGlobalBest().getX() - particles[i].getPosition().getX());
        double vy = inertia * particles[i].getVelocity().getY() + cognitive * (particles[i].getPersonalBest().getY() - particles[i].getPosition().getY()) + social * (Particle::getGlobalBest().getY() - particles[i].getPosition().getY());
        vx = clampVel(vx);
        vy = clampVel(vy);
        particles[i].updateVelocity(vx, vy);

        double x = particles[i].getPosition().getX() + vx;
        double y = particles[i].getPosition().getY() + vy;
        clampPos(&x, lowerBound, upperBound);
        clampPos(&y, lowerBound, upperBound);
        particles[i].updatePosition(x, y);

        if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(particles[i].getPersonalBest())) {
            particles[i].updatePersonalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }

        if (evaluateFunction(particles[i].getPosition()) < evaluateFunction(Particle::getGlobalBest())) {
            Particle::updateGlobalBest(particles[i].getPosition().getX(), particles[i].getPosition().getY());
        }
    }
}
