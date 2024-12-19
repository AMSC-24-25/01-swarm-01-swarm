#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "coordinate.hpp"

class Particle {
private:
    Coordinate position;
    Coordinate velocity;
    Coordinate personalBest;
    static Coordinate globalBest;

public:
    Particle();
    Particle(double x, double y, double vx, double vy);

    void updatePosition(double x, double y);
    void updateVelocity(double x, double y);
    void updatePersonalBest(double newBestx, double newBesty);

    static void updateGlobalBest(double newBestx, double newBesty);
    static Coordinate& getGlobalBest();

    Coordinate getPersonalBest() const;
    Coordinate getPosition() const;
    Coordinate getVelocity() const;
};

#endif // PARTICLE_HPP
