#include <iostream>
#include "particle.hpp"
#include "particle_test.hpp"
#include "functions.hpp"
#include <vector>
#include <string>
#include <sys/time.h>

using namespace std;

void displayMenu() {
    cout << "=== Particle Swarm Optimization Menu ===" << endl;
    cout << "1. Select the function to optimize" << endl;
    cout << "   [1] Rastrigin" << endl;
    cout << "   [2] Sphere" << endl;
    cout << "   [3] Rosenbrock" << endl;
    cout << "   [4] Beale" << endl;
    cout << "   [5] Eggholder" << endl;
    cout << "2. Setup Parameters" << endl;
    cout << "3. Setup Bounds" << endl;
    cout << "4. Launch Simulation" << endl;
    cout << "0. Exit" << endl;
    cout << "========================================" << endl;
}

void printSimulationRecap(int n_particles, int n_iterations, float inertia, float cognitive, float social, float lowerBound, float upperBound) {
    std::cout << "\n=== Simulation Recap ===" << std::endl;
    std::cout << "Selected function: " << getFunctionName() << std::endl;
    std::cout << "Number of particles: " << n_particles << std::endl;
    std::cout << "Number of iterations: " << n_iterations << std::endl;
    std::cout << "Inertia: " << inertia << std::endl;
    std::cout << "Cognitive weight: " << cognitive << std::endl;
    std::cout << "Social weight: " << social << std::endl;
    std::cout << "Bounds: [" << lowerBound << ", " << upperBound << "]" << std::endl;
    std::cout << "================================\n" << std::endl;
}


void selectFunction() {
    int choice;
    cout << "Select a function: ";
    cin >> choice;
    switch (choice) {
        case 1: setCurrentFunction(RASTRIGIN); break;
        case 2: setCurrentFunction(SPHERE); break;
        case 3: setCurrentFunction(ROSENBROCK); break;
        case 4: setCurrentFunction(BAELE); break;
        case 5: setCurrentFunction(STRANGE); break;
        default:
            cerr << "Not valid, selecting Eggholder as default" << endl;
            setCurrentFunction(STRANGE);
    }
}

void setupParameters(int& n_particles, int& n_iterations, float& inertia, float& cognitive, float& social) {
    cout << "Insert particles number: ";
    cin >> n_particles;
    cout << "Insert iteration number: ";
    cin >> n_iterations;
    cout << "Insert inertial weight: ";
    cin >> inertia;
    cout << "Insert cognitive weight: ";
    cin >> cognitive;
    cout << "Insert social weight: ";
    cin >> social;
}

void setupBounds(float& lowerBound, float& upperBound) {
    cout << "Insert(LOWER_BOUND): ";
    cin >> lowerBound;
    cout << "Insert (UPPER_BOUND): ";
    cin >> upperBound;

    if (lowerBound >= upperBound) {
        cerr << "Error: LOWER_BOUND must be less than UPPER_BOUND. Restoring default values." << endl;
        lowerBound = -10.0;
        upperBound = 10.0;
    }
}

int main() {
    int n_particles = 100, n_iterations = 1000;
    float inertia = 0.5, cognitive = 1.5, social = 1.5;
    float lowerBound = -10.0, upperBound = 10.0;

    while (true) {
        displayMenu();
        int choice;
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                selectFunction();
                break;
            case 2:
                setupParameters(n_particles, n_iterations, inertia, cognitive, social);
                break;
            case 3:
                setupBounds(lowerBound, upperBound);
                break;
            case 4: {
                vector<Particle> particles(n_particles);
                setupParticlesParallel(particles, lowerBound, upperBound);
                for (int i = 0; i < n_iterations; i++) {
                    updatesParallel(inertia, cognitive, social, particles, lowerBound, upperBound);
                }
                printSimulationRecap(n_particles, n_iterations, inertia, cognitive, social, lowerBound, upperBound);
                cout << "Best result: (" 
                     << Particle::getGlobalBest().getX() << ", " 
                     << Particle::getGlobalBest().getY() << ") Valore: " 
                     << evaluateFunction(Particle::getGlobalBest()) << endl;
                break;
            }
            case 0:
                cout << "Closing the program..." << endl;
                return 0;
            default:
                cerr << "Choice not valid." << endl;
        }
    }
}