#include "particle.hpp"

// Definizione della variabile statica
Coordinate Particle::globalBest = Coordinate();

Particle::Particle() {
    position.setCoordinate(0, 0);
    velocity.setCoordinate(0, 0);
    personalBest.setCoordinate(0, 0);
}

Particle::Particle(double x, double y, double vx, double vy)
    : position(x, y), velocity(vx, vy), personalBest(x, y) {}

void Particle::updatePosition(double x, double y) {
    position.setCoordinate(x, y);
}

void Particle::updateVelocity(double x, double y) {
    velocity.setCoordinate(x, y);
}

void Particle::updatePersonalBest(double newBestx, double newBesty) {
    personalBest.setCoordinate(newBestx, newBesty);
}

void Particle::updateGlobalBest(double newBestx, double newBesty) {
    globalBest.setCoordinate(newBestx, newBesty);
}

Coordinate& Particle::getGlobalBest() {
    return globalBest;
}

Coordinate Particle::getPersonalBest() const {
    return personalBest;
}

Coordinate Particle::getPosition() const {
    return position;
}

Coordinate Particle::getVelocity() const {
    return velocity;
}
