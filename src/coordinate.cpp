#include "coordinate.hpp"

Coordinate::Coordinate() : x(0), y(0) {}

Coordinate::Coordinate(double x, double y) : x(x), y(y) {}

void Coordinate::setCoordinate(double x, double y) {
    this->x = x;
    this->y = y;
}

double Coordinate::getX() const {
    return x;
}

double Coordinate::getY() const {
    return y;
}
