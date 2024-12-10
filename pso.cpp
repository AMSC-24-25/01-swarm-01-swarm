#include <iostream>
#include <vector>
#include <cmath>
#include <limits>


// function to optimize: here a 3d parabola
double Function(std::vector<double>& x) {
    return x[0]*x[0]+x[1]*x[1];
}

// Class for a single particle
class Particle {
public:
    std::vector<double> v;
    std::vector<double> x;
    std::vector<double> bestx;
    double besty;
   
    Particle(int dimensions, double lowerBound, double upperBound) {
        x.resize(dimensions);
        v.resize(dimensions);
        bestx.resize(dimensions);

        for (int i = 0; i < dimensions; i++) {
            x[i] = lowerBound + static_cast<double>(rand()) / RAND_MAX * (upperBound - lowerBound);
            v[i] = 0.0;
            bestx[i] = x[i];
        }
        besty = Function(x);
    }
};

// Particle Swarm Optimization
void PSO(int numParticles, int dim, int maxIter, double lowerBound, double upperBound) {
    std::vector<Particle> particleSwarm;
    std::vector<double> globalBestx(dim);
    double globalBesty = std::numeric_limits<double>::infinity();

    // Initialize particles and best starting global y
    for (int i = 0; i < numParticles; i++) {
        particleSwarm.emplace_back(dim, lowerBound, upperBound);
        if (particleSwarm[i].besty < globalBesty){
            globalBesty = particleSwarm[i].besty;
        }
    }


    for (int i = 0; i < maxIter; i++) {
        for (int j = 0; j < numParticles; j++) {
            // Update personal best
            if (Function(particleSwarm[j].x) < particleSwarm[j].besty) {
                particleSwarm[j].besty = Function(particleSwarm[j].x);
                particleSwarm[j].bestx = particleSwarm[j].x;
            }

            // Update global best
            if (Function(particleSwarm[j].x) < globalBesty) {
                globalBesty = Function(particleSwarm[j].x);
                globalBestx = particleSwarm[j].x;
            }
        }

        // Update velocity and position
        double w = 0.5; 
        double w1 = 1.5; 
        double w2 = 1.5; 
        for (int j = 0; j < numParticles; j++)  {
            double r1 = static_cast<double>(rand()) / RAND_MAX;
            double r2 = static_cast<double>(rand()) / RAND_MAX;        
            for (int d = 0; d < dim; d++) {
                particleSwarm[j].v[d] = w * particleSwarm[j].v[d]+ w1 * r1 * (particleSwarm[j].bestx[d] - particleSwarm[j].x[d]) + w2 * r2 * (globalBestx[d] - particleSwarm[j].x[d]);
                particleSwarm[j].x[d] += particleSwarm[j].v[d];
                particleSwarm[j].x[d] = std::max(lowerBound, std::min(upperBound, particleSwarm[j].x[d]));
            }
        }
    }
    std::cout << ": Best Y = " << globalBesty << ", Best Position = [";
    for (double val : globalBestx) {
            std::cout << val << " ";
        }
    std::cout << "]\n";
}

int main() {
    int numParticles = 50;
    int dim = 2; // Number of dimensions for the optimization problem
    int maxIter = 100;
    double lowerBound = -5.0;
    double upperBound = 5.0;

    PSO(numParticles, dim, maxIter, lowerBound, upperBound);
    return 0;
}
