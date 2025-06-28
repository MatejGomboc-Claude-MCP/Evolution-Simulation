#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include "biological_sim.h"

/**
 * @class BiologicalEvolutionSimulation
 * @brief Evolution simulation with biological features like energy, mating, and food
 */
class BiologicalEvolutionSimulation {
private:
    World world;
    size_t population_size;
    double mutation_rate;
    size_t food_spawn_rate;
    size_t generation;
    
public:
    BiologicalEvolutionSimulation(size_t pop_size, double mut_rate, size_t food_rate) 
        : population_size(pop_size), mutation_rate(mut_rate), 
          food_spawn_rate(food_rate), generation(0) {
        
        // Create initial population
        for (size_t i = 0; i < population_size; ++i) {
            world.addOrganism();
        }
        
        // Add initial food
        world.addFood(food_spawn_rate * 2);
    }
    
    // Execute programs and let organisms interact
    void simulateInteractions() {
        // Execute each organism's program
        for (auto& organism : world.organisms) {
            try {
                // Execute the program multiple times to allow for complex behaviors
                for (int i = 0; i < 5; ++i) {
                    organism->program.execute();
                }
                
                // Update fitness based on energy and age
                organism->fitness = organism->energy * 0.7 + organism->age * 0.3;
                
            } catch (...) {
                // Programs that crash get lower fitness
                organism->fitness *= 0.5;
            }
        }
        
        // Simulate some random interactions (in a real implementation,
        // these would be driven by the instruction execution)
        simulateRandomInteractions();
    }
    
    // Simulate random biological interactions
    void simulateRandomInteractions() {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<size_t> org_dist(0, world.organisms.size() - 1);
        std::uniform_int_distribution<size_t> food_dist(0, world.food_items.size() - 1);
        std::uniform_real_distribution<double> prob_dist(0.0, 1.0);
        
        // Simulate mating attempts
        for (size_t i = 0; i < world.organisms.size() / 4; ++i) {
            if (world.organisms.size() < 2) break;
            
            size_t org1_idx = org_dist(rng) % world.organisms.size();
            size_t org2_idx = org_dist(rng) % world.organisms.size();
            
            if (org1_idx != org2_idx) {
                Organism* org1 = world.organisms[org1_idx].get();
                Organism* org2 = world.organisms[org2_idx].get();
                
                // Simulate mating request
                if (prob_dist(rng) < 0.3 && org1->hasEnergy(Organism::MATING_ENERGY_COST)) {
                    world.processMatingRequest(org1->id, org2->id);
                    
                    // Simulate acceptance
                    if (prob_dist(rng) < 0.5 && org2->hasEnergy(Organism::MATING_ENERGY_COST)) {
                        world.processMatingAcceptance(org2->id, org1->id);
                    }
                }
            }
        }
        
        // Simulate food consumption attempts
        for (size_t i = 0; i < world.organisms.size() / 2; ++i) {
            if (world.organisms.empty() || world.food_items.empty()) break;
            
            size_t org_idx = org_dist(rng) % world.organisms.size();
            size_t food_idx = food_dist(rng) % world.food_items.size();
            
            Organism* org = world.organisms[org_idx].get();
            Food* food = world.food_items[food_idx].get();
            
            if (!food->consumed) {
                world.consumeFood(org->id, food->id);
            }
        }
        
        // Simulate conflicts
        for (size_t i = 0; i < world.organisms.size() / 10; ++i) {
            if (world.organisms.size() < 2) break;
            
            size_t org1_idx = org_dist(rng) % world.organisms.size();
            size_t org2_idx = org_dist(rng) % world.organisms.size();
            
            if (org1_idx != org2_idx) {
                Organism* org1 = world.organisms[org1_idx].get();
                Organism* org2 = world.organisms[org2_idx].get();
                
                size_t winner_id = world.resolveConflict(org1->id, org2->id);
                
                // Winner gains some energy, loser loses some
                if (winner_id == org1->id) {
                    org1->gainEnergy(5.0);
                    org2->consumeEnergy(10.0);
                } else {
                    org2->gainEnergy(5.0);
                    org1->consumeEnergy(10.0);
                }
            }
        }
    }
    
    // Natural selection based on energy
    void naturalSelection() {
        // Sort organisms by fitness (energy * age factor)
        std::sort(world.organisms.begin(), world.organisms.end(),
            [](const std::unique_ptr<Organism>& a, const std::unique_ptr<Organism>& b) {
                return a->fitness > b->fitness;
            });
        
        // Keep top 50% of organisms
        size_t survivors = world.organisms.size() / 2;
        if (survivors < 10) survivors = std::min(size_t(10), world.organisms.size());
        
        // Remove weakest organisms
        while (world.organisms.size() > survivors) {
            world.organisms.pop_back();
        }
    }
    
    // Reproduce to maintain population
    void reproduce() {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_real_distribution<double> mut_dist(0.0, 1.0);
        std::uniform_int_distribution<size_t> parent_dist(0, world.organisms.size() - 1);
        
        size_t target_pop = population_size;
        
        while (world.organisms.size() < target_pop && !world.organisms.empty()) {
            // Select random parent
            size_t parent_idx = parent_dist(rng) % world.organisms.size();
            Organism* parent = world.organisms[parent_idx].get();
            
            // Create offspring
            Organism* child = world.addOrganism();
            
            // Inherit from parent with possible mutation
            if (mut_dist(rng) < mutation_rate) {
                // Mutate: generate new random program
                child->program = Program::random();
            } else {
                // Inherit: copy parent's program (simplified - should do crossover)
                child->program = parent->program;
            }
            
            // Give child initial energy
            child->energy = Organism::INITIAL_ENERGY * 0.8;
        }
    }
    
    // Print statistics
    void printStats() {
        if (world.organisms.empty()) {
            std::cout << "Generation " << std::setw(4) << generation 
                      << " | Population extinct!" << std::endl;
            return;
        }
        
        double total_energy = 0.0;
        double total_fitness = 0.0;
        double max_energy = 0.0;
        double max_fitness = 0.0;
        size_t max_age = 0;
        size_t total_age = 0;
        
        for (const auto& organism : world.organisms) {
            total_energy += organism->energy;
            total_fitness += organism->fitness;
            max_energy = std::max(max_energy, organism->energy);
            max_fitness = std::max(max_fitness, organism->fitness);
            max_age = std::max(max_age, organism->age);
            total_age += organism->age;
        }
        
        double avg_energy = total_energy / world.organisms.size();
        double avg_fitness = total_fitness / world.organisms.size();
        double avg_age = static_cast<double>(total_age) / world.organisms.size();
        
        std::cout << "Generation " << std::setw(4) << generation 
                  << " | Pop: " << std::setw(3) << world.organisms.size()
                  << " | Avg Energy: " << std::fixed << std::setprecision(1) << avg_energy
                  << " | Max Energy: " << std::fixed << std::setprecision(1) << max_energy
                  << " | Avg Age: " << std::fixed << std::setprecision(1) << avg_age
                  << " | Max Age: " << max_age
                  << " | Food: " << world.food_items.size()
                  << std::endl;
    }
    
    // Run the simulation
    void run(size_t num_generations) {
        std::cout << "Starting Biological Evolution Simulation" << std::endl;
        std::cout << "Initial population: " << population_size << std::endl;
        std::cout << "Mutation rate: " << mutation_rate << std::endl;
        std::cout << "Food spawn rate: " << food_spawn_rate << " per generation" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (generation = 0; generation < num_generations; ++generation) {
            // Spawn new food
            world.addFood(food_spawn_rate);
            
            // Simulate interactions
            simulateInteractions();
            
            // Update world (deplete energy, remove dead)
            world.update();
            
            // Natural selection
            if (world.organisms.size() > 0) {
                naturalSelection();
            }
            
            // Print statistics
            printStats();
            
            // Check for extinction
            if (world.organisms.empty()) {
                std::cout << "Population extinct at generation " << generation << "!" << std::endl;
                break;
            }
            
            // Reproduce to maintain population
            reproduce();
        }
        
        std::cout << std::string(80, '-') << std::endl;
        std::cout << "Biological evolution simulation completed!" << std::endl;
        
        // Final statistics
        if (!world.organisms.empty()) {
            std::cout << "Final population size: " << world.organisms.size() << std::endl;
            std::cout << "Oldest organism age: ";
            size_t max_age = 0;
            for (const auto& org : world.organisms) {
                max_age = std::max(max_age, org->age);
            }
            std::cout << max_age << " generations" << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    // Default parameters
    size_t population_size = 50;
    double mutation_rate = 0.2;
    size_t generations = 100;
    size_t food_spawn_rate = 20;
    
    // Parse command line arguments
    if (argc > 1) population_size = std::stoul(argv[1]);
    if (argc > 2) mutation_rate = std::stod(argv[2]);
    if (argc > 3) generations = std::stoul(argv[3]);
    if (argc > 4) food_spawn_rate = std::stoul(argv[4]);
    
    std::cout << "Biological Evolution Simulation - Genetic Programming with Life" << std::endl;
    std::cout << "==============================================================" << std::endl;
    
    try {
        BiologicalEvolutionSimulation sim(population_size, mutation_rate, food_spawn_rate);
        sim.run(generations);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
