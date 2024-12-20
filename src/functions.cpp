#include "functions.hpp"
#include <cmath>
#include <iostream>

using namespace std;


/*
A good attempt to be able to treat different functions. However it is more elegant and
more easily extensible using a factory that employes a map or a unordered_map, per esempio
struct FunctionFactory{
void addFunction(FunctionType const & f, std::string const & id)
 functions.insert(id,f);
auto getFunction(std::string const & name) const
{
   auto it=functions.find(name);
   if (it == functions.end())
     throw "Function not found";
   return it->second;
}
private:
  std::map<std::string, FunctionType> functions;
}

FunctionFactory loadFunctions()
{
    FunctionFactory factory;
    factory.addFunction("Rosenbrock". &rosembrock)'

// etc etc
}
This is the general idea of an object factory that maps an identifier to an object.
    */
static FunctionType currentFunction = STRANGE;
std::string getFunctionName() {
    switch (getCurrentFunction()) {
        case RASTRIGIN: return "Rastrigin";
        case SPHERE: return "Sphere";
        case ROSENBROCK: return "Rosenbrock";
        case BAELE: return "Beale";
        case STRANGE: return "Eggholder";
        default: return "Funzione sconosciuta";
    }
}


FunctionType getCurrentFunction() {
    return currentFunction;
}


void setCurrentFunction(FunctionType func) {
    currentFunction = func;
}



double eggholder(Coordinate x) {
    return -(x.getY() + 47) * sin(sqrt(abs(x.getY() + x.getX() / 2 + 47))) - x.getX() * sin(sqrt(abs(x.getX() - (x.getY() + 47))));
}

double simionescu(Coordinate x) {
    return 0.5 + (pow(sin(sqrt(pow(x.getX(), 2) + pow(x.getY(), 2))), 2) - 0.5 / (1 + 0.001 * (pow(x.getX(), 2) + pow(x.getY(), 2))));
}

double rastrigin(Coordinate x) {
    return 20 + pow(x.getX(), 2) + pow(x.getY(), 2) - 10 * (cos(2 * M_PI * x.getX()) + cos(2 * M_PI * x.getY()));
}

double strangeFunction(Coordinate x) {
    return pow(pow(x.getX(), 2) + x.getY() - 11, 2) + pow(pow(x.getY(), 2) + x.getX() - 7, 2);
}

double baele(Coordinate x) {
    return pow(1.5 - x.getX() + x.getX() * x.getY(), 2) + pow(2.25 - x.getX() + x.getX() * pow(x.getY(), 2), 2) + pow(2.625 - x.getX() + x.getX() * pow(x.getY(), 3), 2);
}

double sphere(Coordinate x) {
    return x.getX() * x.getX() + x.getY() * x.getY();
}

double rosenbrock(Coordinate x) {
    return 100 * pow(x.getY() - pow(x.getX(), 2), 2) + pow(1 - x.getX(), 2);
}

double evaluateFunction(Coordinate x) {
    switch (currentFunction) {
        case RASTRIGIN: return rastrigin(x);
        case SPHERE: return sphere(x);
        case ROSENBROCK: return rosenbrock(x);
        case BAELE: return baele(x);
        case STRANGE: return eggholder(x);
        default:
            cerr << "Funzione non riconosciuta!" << endl;
            exit(1);
    }
}
