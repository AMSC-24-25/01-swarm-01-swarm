#include <iostream>
#include "particle.hpp"
#include "particle_test.hpp"
#include "functions.hpp"
#include <vector>
#include <string>
#include <sys/time.h>
#include <chrono>
#include <functional>
#include <fstream>
#include <cmath>
#include <numeric>
using namespace std;


double measureExecutionTime(
    const std::string& methodName,
    const std::function<void()>& simulationFunction
) {
    auto start = std::chrono::high_resolution_clock::now();
    simulationFunction();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Execution time (" << methodName << "): " << elapsed.count() << " seconds" << std::endl;
    return elapsed.count();
}

double calculateError(Coordinate globalBestValue, Coordinate optimalValue) {
    // Calculate the error as the Euclidean distance between the global best value and the optimal value
    return sqrt(pow(globalBestValue.getX() - optimalValue.getX(), 2) + pow(globalBestValue.getY() - optimalValue.getY(), 2));
}
void runGraphExperiments() {
    std::ofstream speedupFile("speedup_data.csv");
    speedupFile << "function,particles,iterations,speedup\n";

    std::ofstream errorFile("error_data.csv");
    errorFile << "function,iterations,error\n";

    float lowerBound = -512, upperBound = 512;
    float inertia = 0.5, cognitive = 0.5, social = 0.5;

    std::vector<std::pair<FunctionType, Coordinate>> functions = {
        {ROSENBROCK, Coordinate(1, 1)},
        {BAELE, Coordinate(3, 0.5)},
        {STRANGE, Coordinate(512, 404.2319)},
    };

    for (const auto& func : functions) {
        setCurrentFunction(func.first);
        std::string functionName = getFunctionName();
        

        
        for (int n_particles = 20; n_particles <= 1520; n_particles += 50) {
            double totalParallelTime = 0, totalSerialTime = 0;

            for (int run = 0; run < 100; ++run) {
                std::vector<Particle> particlesParallel(n_particles);
                std::vector<Particle> particlesSerial(n_particles);

                totalParallelTime += measureExecutionTime("Parallel", [&]() {
                    setupParticlesParallel(particlesParallel, lowerBound, upperBound);
                    for (int i = 0; i < 50; ++i) {
                        updatesParallel(inertia, cognitive, social, particlesParallel, lowerBound, upperBound);
                    }
                });

                totalSerialTime += measureExecutionTime("Serial", [&]() {
                    setupParticlesSerial(particlesSerial, lowerBound, upperBound);
                    for (int i = 0; i < 50; ++i) {
                        updatesSerial(inertia, cognitive, social, particlesSerial, lowerBound, upperBound);
                    }
                });
            }

            double avgSpeedup = (totalSerialTime / 30) / (totalParallelTime / 30);
            speedupFile << functionName << "," << n_particles << ",50," << avgSpeedup << "\n";
        }

        
        for (int iterations = 50; iterations <= 1000; iterations += 50) {
            double totalParallelTime = 0, totalSerialTime = 0;

            for (int run = 0; run < 100; ++run) {
                std::vector<Particle> particlesParallel(50);
                std::vector<Particle> particlesSerial(50);

                totalParallelTime += measureExecutionTime("Parallel", [&]() {
                    setupParticlesParallel(particlesParallel, lowerBound, upperBound);
                    for (int i = 0; i < iterations; ++i) {
                        updatesParallel(inertia, cognitive, social, particlesParallel, lowerBound, upperBound);
                    }
                });

                totalSerialTime += measureExecutionTime("Serial", [&]() {
                    setupParticlesSerial(particlesSerial, lowerBound, upperBound);
                    for (int i = 0; i < iterations; ++i) {
                        updatesSerial(inertia, cognitive, social, particlesSerial, lowerBound, upperBound);
                    }
                });
            }

            double avgSpeedup = (totalSerialTime / 30) / (totalParallelTime / 30);
            speedupFile << functionName << ",50," << iterations << "," << avgSpeedup << "\n";
        }

        
        for (int iterations = 1; iterations <= 301; iterations += 10) {
            
            double error=0;
            
            for(int run=0;run<100;run++){
                std::vector<Particle> particles(50);
                setupParticlesSerial(particles, lowerBound, upperBound);

                for (int i = 0; i < iterations; ++i) {
                    updatesSerial(inertia, cognitive, social, particles, lowerBound, upperBound);
                }
                error += calculateError(Particle::getGlobalBest(), func.second);
            }
            error = error/100;
            errorFile << functionName << "," << iterations << "," << error << "\n";
            
        }
    }

    speedupFile.close();
    errorFile.close();
    system("python3 generate_graphs.py");
}



void runExperiment() {
    int n_iterations = 15; 
    float inertia = 0.5, cognitive = 0.5, social = 0.5;
    float lowerBound = -512, upperBound = 512; 

    std::ofstream results("results.csv");
    results << "function,n_particles,error\n";

    

    std::vector<std::pair<FunctionType, Coordinate>> functions = {
        {ROSENBROCK, Coordinate(1, 1)},
        {BAELE, Coordinate(3, 0.5)},
        {STRANGE, Coordinate(512, 404.2319)},
    };

    for (const auto& func : functions) {
        setCurrentFunction(func.first);
        Coordinate optimalValue = func.second;

        for (int n_particles = 100; n_particles <= 3000; n_particles += 100) {
            std::vector<Particle> particles(n_particles);
            setupParticlesSerial(particles, lowerBound, upperBound);

            for (int i = 0; i < n_iterations; i++) {
                updatesSerial(inertia, cognitive, social, particles, lowerBound, upperBound);
            }

            double error = calculateError(Particle::getGlobalBest(), optimalValue);
            results << getFunctionName() << "," << n_particles << "," << error << "\n";

            particles.clear();
        }
    }

    results.close();
    system("python3 experiment.py");
}



void saveParticlePositions(const std::vector<Particle>& particles, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    for (const auto& particle : particles) {
        file << particle.getPosition().getX() << "," << particle.getPosition().getY() << "\n";
    }
    file << "END\n"; 
    file.close();
}

void saveFunctionValues(const std::string& filename, float lowerBound, float upperBound, int resolution) {
    std::ofstream file(filename);
    float step = (upperBound - lowerBound) / (resolution - 1);

    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            float x = lowerBound + i * step;
            float y = lowerBound + j * step;
            float value = evaluateFunction(Coordinate(x, y));
            file << x << "," << y << "," << value << "\n";
        }
    }

    file.close();
}





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
    cout << "5. Generate Simulation GIF" << endl;
    cout << "6. Clean GIF files" << endl;
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
                std::vector<Particle> particles(n_particles);
                std::vector<Particle> particlesSerial(particles);

                

                printSimulationRecap(n_particles, n_iterations, inertia, cognitive, social, lowerBound, upperBound);

                double parallelTime=measureExecutionTime("Parallel", [&]() {
                    setupParticlesParallel(particles, lowerBound, upperBound);
                    for (int i = 0; i < n_iterations; i++) {
                        updatesParallel(inertia, cognitive, social, particles, lowerBound, upperBound);
                    }
                });
                std::cout << "Best result (Parallel): ("
                          << Particle::getGlobalBest().getX() << ", "
                          << Particle::getGlobalBest().getY() << ") Valore: "
                          << evaluateFunction(Particle::getGlobalBest()) << std::endl;
                
                //free memory
                particles.clear();

                double serialTime=measureExecutionTime("Serial", [&]() {
                    setupParticlesSerial(particlesSerial, lowerBound, upperBound);
                    for (int i = 0; i < n_iterations; i++) {
                        updatesSerial(inertia, cognitive, social, particlesSerial, lowerBound, upperBound);
                    }
                });

                

                
                std::cout << "Best result (Serial): ("
                          << Particle::getGlobalBest().getX() << ", "
                          << Particle::getGlobalBest().getY() << ") Valore: "
                          << evaluateFunction(Particle::getGlobalBest()) << std::endl;
                cout<<"Speedup: "<<serialTime/parallelTime<<endl;
                particlesSerial.clear();
                break;
            }
            case 5: { 
                std::vector<Particle> particles(n_particles);
                setupParticlesSerial(particles, lowerBound, upperBound);

                saveFunctionValues("./animation/function_values.csv", lowerBound, upperBound, 200);

                for (int i = 0; i < n_iterations; i++) {
                    updatesSerial(inertia, cognitive, social, particles, lowerBound, upperBound);
                    saveParticlePositions(particles, "./animation/particle_positions.csv");
                }

                
                system("python3 pso_animation.py");

                break;
            }
            case 6: {
                system("rm ./animation/*.csv ./frames/*.png");
                system("rm pso_animation.gif");
                break;
            }
            case 7:
                runExperiment();
                break;
            case 8:
                runGraphExperiments();
                break;
            case 0:
                cout << "Closing the program..." << endl;
                return 0;
            default:
                cerr << "Choice not valid." << endl;
        }
    }
}
