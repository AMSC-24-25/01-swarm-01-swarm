#include <iostream>
#include <vector>
#include <cmath>
#include "coordinate.hpp"

class Particle {
    private: 
        Coordinate position;
        Coordinate velocity;
        double personalBest;
        static Coordinate globalBest;

    public: 
        Particle(double x, double y, double vx, double vy){
            position.setCoordinate(x, y);
            velocity.setCoordinate(vx, vy);
            personalBest = 0;
        }

        void updatePosition(){
            position.setCoordinate();
        }
        void updateVelocity(){
            velocity.setCoordinate();
        }
        void updatePersonalBest(double newBest){
            personalBest = newBest;
        }
}