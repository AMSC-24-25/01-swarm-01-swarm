# SwarmSearch: Optimization by Swarm Search

## Overview
The **Particle Swarm Optimization (PSO)** algorithm replicates the behavior of a swarm of birds searching for the optimal place to build their nest. Each bird in the swarm is represented as a **particle** in the \( R^3 \) space:

- **x** and **y** coordinates are initialized randomly.
- **z** coordinate represents the evaluation of the function \( f(x, y) \) to be optimized.

Each particle has information about its position and velocity. During every iteration, particles are updated using information about themselves and the swarm. This document describes a naive implementation of PSO, parallelization techniques, and practical usage details.

---

## Naive Implementation of the Algorithm
In the original version of PSO, the update formulas for each iteration are as follows:

- **Velocity update:**
  ```
  particleVelocity = inertia * particleVelocity + cognitive * (particleBest - particlePosition) + social * (globalBest - particlePosition);
  ```

- **Position update:**
  ```
  particlePosition = particlePosition + particleVelocity;
  ```

Here:
- `inertia`, `cognitive`, and `social` are random parameters.
- `particleBest` represents the particle's personal best position.
- `globalBest` represents the best position found by the swarm.

---

## Parallelization of the Algorithm
We utilized **OpenMP** to parallelize the algorithm for better performance:

- **Setup and Updates Parallelization:**
  - The main functions `setupParticlesParallel` and `updatesParallel` were parallelized using OpenMP directives.
  - The **`#pragma omp parallel for`** directive was applied to the loops.

- **Avoiding Data Races:**
  - The **`#pragma omp critical`** directive was used to safely evaluate the global minimum, ensuring only one thread executes this section at a time.

- **Dynamic Scheduling:**
  - The loop iterations were dynamically assigned based on `CHUNK_SIZE` defined at the beginning of the code.

- **Maximum Threads:**
  - We utilized all available threads with `omp_get_max_threads()`.

---

## Usage of the Application
To run the application:

1. Navigate to the project folder in the terminal.
2. Execute the command:
   ```
   build/main
   ```

3. A menu will appear, allowing the user to:
   - Modify parameters such as:
     - Number of particles.
     - Number of iterations.
     - Inertia, cognitive, and social weights.
     - \( R^3 \) bounds.
     - Function to evaluate.
   - Run the simulation.

The application outputs:
- **Speedup ratio:** The ratio of `serialTime / parallelTime` for comparison between the serial and parallel implementations.

---

## Empirical Observations
From our experiments:

- **Optimal Particle Count:**
  - Literature suggests using **50-100 particles** for optimal performance.

- **Difficult Functions:**
  - For complex functions with many local minima (e.g., the Eggholder function), a large number of particles is required. This significantly slows down the serial version but highlights the advantage of the parallel implementation.

- **Simple Functions:**
  - Simpler functions converge quickly with fewer particles and iterations, making the parallel implementation less advantageous compared to the serial one.

---

## File Descriptions

### `coordinate.cpp`
Defines a vector of coordinates used for:
- Particle positions.
- Particle velocities.

### `particle.cpp`
Defines the **Particle Class**, including:
- **State variables:** Position, velocity, and personal best position.
- **Global state:** A static variable representing the global best position.

### `particle_test.cpp`
Contains the main implementation of the Particle Swarm Optimization algorithm:
- **Setup function:** Initializes the swarm.
- **Update function:** Updates particle information to solve the optimization problem.
- **Clamping function** Makes sure to not overstep the boundaries of both the position and the velocity of each particle.

### `functions.cpp`
Defines the functions to be optimized, showcasing:
- Strengths and weaknesses of the PSO algorithm.

### `main.cpp`
Defines the user interface and simulation flow:
- **Menu options:** Allows users to select the function, parameters, and bounds.
- **Simulation results:** Outputs a comparison of the serial and parallel implementations.
- **GIF Generation:** Provides an option to visualize the simulation (requires Python3 with `matplotlib`, `pandas`, `Pillow`, and `numpy`).

---

## Conclusion
This implementation demonstrates the capabilities of Particle Swarm Optimization with both serial and parallel approaches. While the algorithm performs well for a variety of optimization problems, its effectiveness depends on the choice of parameters and the complexity of the function being optimized. The integration of OpenMP provides significant speedup for larger, more complex simulations.
