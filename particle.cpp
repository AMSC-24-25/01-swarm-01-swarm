#include <iostream>
#include <vector>
#include <cmath>
#include "coordinate.cpp"

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
        Particle(double x, double y, double vx, double vy): position(x,y), velocity(vx,vy), personalBest(x,y){}
        void updatePosition(double x, double y){
            position.setCoordinate(x,y);
        }
        void updateVelocity(double x, double y){
            velocity.setCoordinate(x,y);
        }
        void updatePersonalBest(double newBestx, double newBesty){
            personalBest.setCoordinate(newBestx, newBesty);
        }
        static void updateGlobalBest(double newBestx, double newBesty){
            globalBest.setCoordinate(newBestx, newBesty);
        }
        static Coordinate& getGlobalBest(){
            return globalBest;
        }
        Coordinate getPersonalBest() const {
            return personalBest;
        }
        Coordinate getPosition() const {
            return position;
        }
        Coordinate getVelocity() const {
            return velocity;
        }
        
};