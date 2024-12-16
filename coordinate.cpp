#include <iostream>
#include <vector>
#include <cmath>

class Coordinate{
    private: 
        double x;
        double y;

    public: 
        Coordinate(){
            x = 0;
            y = 0;
        }
        Coordinate(double x, double y){
            this->x = x;
            this->y = y;
        }
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