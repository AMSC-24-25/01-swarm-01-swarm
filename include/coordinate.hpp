#ifndef COORDINATE_HPP
#define COORDINATE_HPP

class Coordinate {
private:
    double x;
    double y;

public:
    Coordinate();
    Coordinate(double x, double y);

    void setCoordinate(double x, double y);

    double getX() const;
    double getY() const;
};

#endif // COORDINATE_HPP
