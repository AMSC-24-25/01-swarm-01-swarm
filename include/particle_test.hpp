#ifndef PARTICLE_TESTS_HPP
#define PARTICLE_TESTS_HPP

#include "particle.hpp"
#include <vector>


void setupParticlesParallel(std::vector<Particle>& particles, float lowerBound, float upperBound);
void setupParticlesSerial(std::vector<Particle>& particles, float lowerBound, float upperBound);


void updatesParallel(float inertia, float cognitive, float social, std::vector<Particle>& particles, float lowerBound, float upperBound);
void updatesSerial(float inertia, float cognitive, float social, std::vector<Particle>& particles, float lowerBound, float upperBound);

#endif // PARTICLE_TESTS_HPP
