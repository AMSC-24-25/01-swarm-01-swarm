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
        Particle(){
            position.setCoordinate(0,0);
            velocity.setCoordinate(0,0);
            personalBest = 0;
        } 
        Particle(double x, double y, double vx, double vy){
            position.setCoordinate(x, y);
            velocity.setCoordinate(vx, vy);
            personalBest = 0;
        }

        void updatePosition(double x, double y){
            position.setCoordinate(x,y);
        }
        void updateVelocity(double x, double y){
            velocity.setCoordinate(x,y);
        }
        void updatePersonalBest(double newBest){
            personalBest = newBest;
        }
        Coordinate getPosition(){
            return position;
        }
        
};