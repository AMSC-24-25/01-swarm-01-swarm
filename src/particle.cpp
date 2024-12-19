#include "particle.hpp"

Coordinate Particle::globalBest = Coordinate();

/* More elegant, and more efficient:
Particle::Particle():position{0.0.}, velocity{0,0},personaleBest{0,0}{};
Moreover, if you set (0,0) as the default value in the declaration of Coordinate you
can use here the synthesised default constructor
*/

Particle::Particle(): position(0,0), velocity(0,0), personalBest(0,0){}


// Here is ok...
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
