#ifndef COORDINATE_HPP
#define COORDINATE_HPP
/*
state assumendo di trattare solo probelmi bidimensionali, ma in realt' il metodo e` estendibile a domini di simensioen arbitraria.
Si poteva fare facilmente usando dei vector o dei tempaltes.

    */
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
