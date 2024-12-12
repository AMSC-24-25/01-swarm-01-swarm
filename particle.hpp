#include <iostream>
#include <vector>
#include <cmath>
#include "coordinate.hpp"

class Particle {
    private: 
        Coordinate position;
        Coordinate velocity;
        Coordinate personalBest;
        static Coordinate globalBest;

    public:
        Particle(){
            position.setCoordinate(0,0);
            velocity.setCoordinate(0,0);
            personalBest.setCoordinate(0,0);
        } 
        Particle(double x, double y, double vx, double vy){
            position.setCoordinate(x, y);
            velocity.setCoordinate(vx, vy);
            personalBest.setCoordinate(0,0);
        }

        void updatePosition(double x, double y){
            position.setCoordinate(x,y);
        }
        void updateVelocity(double x, double y){
            velocity.setCoordinate(x,y);
        }
        void updatePersonalBest(double newBestx, double newBesty){
            personalBest.setCoordinate(newBestx, newBesty);
        }
        Coordinate getGlobalBest(){
            return globalBest;
        }
        Coordinate getPersonalBest(){
            return personalBest;
        }
        Coordinate getPosition(){
            return position;
        }
        Coordinate getVelocity(){
            return velocity;
        }
        
};