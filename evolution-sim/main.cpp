#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include "program.h"

// Organism class that wraps a Program with fitness information
class Organism {
public:
    Program program;
    double fitness;
    
    Organism() : fitness(0.0) {
        program = Program::random();
    }
    
    // Copy constructor
    Organism(const Organism& other) : program(other.program), fitness(other.fitness) {}
};

// Evolution simulation class
class EvolutionSimulation {
private:
    std::vector<Organism> population;
    size_t population_size;
    double mutation_rate;
    std::mt19937 rng;
    size_t generation;
    
public:
    EvolutionSimulation(size_t pop_size, double mut_rate) 
        : population_size(pop_size), mutation_rate(mut_rate), generation(0) {
        // Initialize random number generator
        auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        rng.seed(seed);
        
        // Create initial population
        population.reserve(population_size);
        for (size_t i = 0; i < population_size; ++i) {
            population.emplace_back();
        }
    }
    
    // Evaluate fitness of all organisms
    void evaluateFitness() {
        for (auto& organism : population) {
            // Execute the program multiple times and evaluate performance
            double total_score = 0.0;
            const int trials = 10;
            
            for (int trial = 0; trial < trials; ++trial) {
                try {
                    // Execute the program
                    organism.program.execute();
                    
                    // Simple fitness function: programs that execute without errors get points
                    // In a real simulation, this would evaluate the program's behavior
                    total_score += 1.0;
                    
                    // You could add more sophisticated fitness metrics here:
                    // - Check if the program solved a specific problem
                    // - Evaluate the efficiency of the solution
                    // - Check memory usage patterns
                    // - Evaluate mathematical computations
                    
                } catch (...) {
                    // Programs that crash get lower fitness
                    total_score += 0.1;
                }
            }
            
            organism.fitness = total_score / trials;
        }
    }
    
    // Tournament selection
    Organism& selectParent() {
        const size_t tournament_size = 5;
        std::uniform_int_distribution<size_t> dist(0, population_size - 1);
        
        size_t best_idx = dist(rng);
        double best_fitness = population[best_idx].fitness;
        
        for (size_t i = 1; i < tournament_size; ++i) {
            size_t idx = dist(rng);
            if (population[idx].fitness > best_fitness) {
                best_idx = idx;
                best_fitness = population[idx].fitness;
            }
        }
        
        return population[best_idx];
    }
    
    // Create next generation
    void evolve() {
        std::vector<Organism> new_population;
        new_population.reserve(population_size);
        
        // Keep best organism (elitism)
        auto best_it = std::max_element(population.begin(), population.end(),
            [](const Organism& a, const Organism& b) { return a.fitness < b.fitness; });
        new_population.push_back(*best_it);
        
        // Create rest of new population
        while (new_population.size() < population_size) {
            // Select parent and create offspring
            Organism& parent = selectParent();
            Organism offspring(parent);
            
            // Apply mutation
            std::uniform_real_distribution<double> mut_dist(0.0, 1.0);
            if (mut_dist(rng) < mutation_rate) {
                // Mutate by generating a new random program
                // In a more sophisticated implementation, you would mutate individual instructions
                offspring.program = Program::random();
            }
            
            new_population.push_back(offspring);
        }
        
        population = std::move(new_population);
        generation++;
    }
    
    // Print statistics
    void printStats() {
        double total_fitness = 0.0;
        double max_fitness = 0.0;
        double min_fitness = std::numeric_limits<double>::max();
        
        for (const auto& organism : population) {
            total_fitness += organism.fitness;
            max_fitness = std::max(max_fitness, organism.fitness);
            min_fitness = std::min(min_fitness, organism.fitness);
        }
        
        double avg_fitness = total_fitness / population_size;
        
        std::cout << "Generation " << std::setw(4) << generation 
                  << " | Avg: " << std::fixed << std::setprecision(4) << avg_fitness
                  << " | Max: " << std::fixed << std::setprecision(4) << max_fitness
                  << " | Min: " << std::fixed << std::setprecision(4) << min_fitness
                  << std::endl;
    }
    
    // Run the simulation
    void run(size_t num_generations) {
        std::cout << "Starting Evolution Simulation" << std::endl;
        std::cout << "Population size: " << population_size << std::endl;
        std::cout << "Mutation rate: " << mutation_rate << std::endl;
        std::cout << "Generations: " << num_generations << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        for (size_t gen = 0; gen < num_generations; ++gen) {
            evaluateFitness();
            printStats();
            evolve();
        }
        
        // Final evaluation
        evaluateFitness();
        printStats();
        
        std::cout << std::string(60, '-') << std::endl;
        std::cout << "Evolution simulation completed!" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    // Default parameters
    size_t population_size = 100;
    double mutation_rate = 0.1;
    size_t generations = 50;
    
    // Parse command line arguments
    if (argc > 1) population_size = std::stoul(argv[1]);
    if (argc > 2) mutation_rate = std::stod(argv[2]);
    if (argc > 3) generations = std::stoul(argv[3]);
    
    std::cout << "Evolution Simulation - Genetic Programming Framework" << std::endl;
    std::cout << "===================================================" << std::endl;
    
    try {
        EvolutionSimulation sim(population_size, mutation_rate);
        sim.run(generations);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
