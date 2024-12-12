#include <iostream>
#include <vector>
#include <cmath>
#include <coordinate.hpp>

class Particle {
    private Coordinate position;
    private Coordinate velocity;
    private double personalBest;
    private static Coordinate globalBest;

    public Particle(double x, double y, double vx, double vy){
        position.setCoordinate(x, y);
        velocity.setCoordinate(vx, vy);
        personalBest = 0;
    }

    public void updatePosition(){
        position.setCoordinate();
    }
    public void updateVelocity(){
        velocity.setCoordinate();
    }
    public void updatePersonalBest(double newBest){
        personalBest = newBest;
    }
}