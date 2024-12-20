#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "coordinate.hpp"
#include <string>

enum FunctionType { RASTRIGIN, SPHERE, ROSENBROCK, BAELE, STRANGE };

FunctionType getCurrentFunction();
std::string getFunctionName();
void setCurrentFunction(FunctionType func);


double eggholder(Coordinate x);
double simionescu(Coordinate x);
double rastrigin(Coordinate x);
double strangeFunction(Coordinate x);
double baele(Coordinate x);
double sphere(Coordinate x);
double rosenbrock(Coordinate x);


double evaluateFunction(Coordinate x);

#endif // FUNCTIONS_HPP
