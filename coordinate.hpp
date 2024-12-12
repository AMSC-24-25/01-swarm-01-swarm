#include <iostream>
#include <vector>
#include <cmath>

class Coordinate{
    private double x;
    private double y;

    public setCoordinate(double x, double y){
        this.x = x;
        this.y = y;
    }
    public double getX(){
        return x;
    }
    public double getY(){
        return y;
    }
}