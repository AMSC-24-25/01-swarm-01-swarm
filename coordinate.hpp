#include <iostream>
#include <vector>
#include <cmath>

class Coordinate{
    private: 
        double x;
        double y;

    public: 
        void setCoordinate(double x, double y){
            this->x = x;
            this->y = y;
        }
        double getX(){
            return x;
        }
        double getY(){
            return y;
        }
};