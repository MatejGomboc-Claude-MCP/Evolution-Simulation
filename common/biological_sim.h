#ifndef BIOLOGICAL_SIM_H
#define BIOLOGICAL_SIM_H

#include <vector>
#include <memory>
#include <random>
#include <optional>
#include "program.h"

// Forward declarations
class Organism;
class Food;
class World;

/**
 * @brief Represents the choice in rock-paper-scissors game
 */
enum class RPSChoice {
    ROCK = 0,
    PAPER = 1,
    SCISSORS = 2
};

/**
 * @brief Result of rock-paper-scissors game
 */
enum class RPSResult {
    PLAYER1_WINS,
    PLAYER2_WINS,
    TIE
};

/**
 * @brief Represents a mating request between organisms
 */
struct MatingRequest {
    size_t requester_id;
    size_t target_id;
    bool accepted = false;
};

/**
 * @class Organism
 * @brief Extended organism class with energy and biological behaviors
 */
class Organism {
public:
    Program program;
    double fitness;
    double energy;
    size_t id;
    size_t age;
    std::optional<MatingRequest> pending_mating_request;
    std::vector<size_t> mating_requests_received;
    
    static constexpr double INITIAL_ENERGY = 100.0;
    static constexpr double ENERGY_DEPLETION_RATE = 1.0;
    static constexpr double MATING_ENERGY_COST = 20.0;
    static constexpr double MUTATION_RATE = 0.1;  // Chance to mutate each instruction
    
    Organism(size_t organism_id);
    
    /**
     * @brief Deplete energy over time
     * @return true if organism is still alive, false if dead
     */
    bool depleteEnergy();
    
    /**
     * @brief Check if organism has enough energy for an action
     * @param required_energy Energy required for the action
     * @return true if organism has enough energy
     */
    bool hasEnergy(double required_energy) const;
    
    /**
     * @brief Consume energy for an action
     * @param amount Amount of energy to consume
     */
    void consumeEnergy(double amount);
    
    /**
     * @brief Gain energy from food
     * @param amount Amount of energy to gain
     */
    void gainEnergy(double amount);
    
    /**
     * @brief Apply random mutations to the organism's program
     * @param rng Random number generator
     */
    void mutate(std::mt19937& rng);
};

/**
 * @class Food
 * @brief Represents food that organisms can consume
 */
class Food {
private:
    std::mt19937* world_rng;  // Pointer to world's RNG
    
public:
    size_t id;
    double energy_value;
    bool consumed;
    static constexpr double DEFAULT_ENERGY = 30.0;
    
    Food(size_t food_id, std::mt19937* rng, double energy = DEFAULT_ENERGY);
    
    /**
     * @brief Attempt to consume this food, resolving conflicts if necessary
     * @param organisms Vector of organisms attempting to consume
     * @return ID of organism that gets to consume, or -1 if none
     */
    size_t resolveConsumptionConflict(const std::vector<Organism*>& organisms);
    
    /**
     * @brief Make RPS choice for an organism (used in conflict resolution)
     * @param organism The organism making the choice
     * @return Rock, paper, or scissors choice
     */
    RPSChoice makeRPSChoice(const Organism* organism) const;
    
    /**
     * @brief Play rock-paper-scissors game
     * @param player1 First player choice
     * @param player2 Second player choice
     * @return Result of the game
     */
    static RPSResult playRockPaperScissors(RPSChoice player1, RPSChoice player2);
    
    /**
     * @brief Consume this food
     * @return Energy value if successful, 0 if already consumed
     */
    double consume();
};

/**
 * @class World
 * @brief The simulation world containing organisms and food
 */
class World {
private:
    std::mt19937 rng;
    
public:
    std::vector<std::unique_ptr<Organism>> organisms;
    std::vector<std::unique_ptr<Food>> food_items;
    size_t next_organism_id;
    size_t next_food_id;
    
    World();
    
    /**
     * @brief Add a new organism to the world
     * @param apply_initial_mutations Whether to apply mutations at creation
     * @return Pointer to the added organism
     */
    Organism* addOrganism(bool apply_initial_mutations = true);
    
    /**
     * @brief Add food to the world
     * @param count Number of food items to add
     */
    void addFood(size_t count);
    
    /**
     * @brief Process mating request between organisms
     * @param requester_id ID of organism making request
     * @param target_id ID of target organism
     * @return true if request was registered successfully
     */
    bool processMatingRequest(size_t requester_id, size_t target_id);
    
    /**
     * @brief Process mating acceptance
     * @param acceptor_id ID of organism accepting
     * @param requester_id ID of original requester
     * @return Pointer to child organism if successful, nullptr otherwise
     */
    Organism* processMatingAcceptance(size_t acceptor_id, size_t requester_id);
    
    /**
     * @brief Attempt to consume food
     * @param organism_id ID of organism attempting to eat
     * @param food_id ID of food to consume
     * @return true if successful
     */
    bool consumeFood(size_t organism_id, size_t food_id);
    
    /**
     * @brief Handle multiple organisms trying to consume the same food
     * @param food_id ID of food being contested
     * @param organism_ids IDs of organisms competing for the food
     */
    void resolveMultipleFoodConsumption(size_t food_id, const std::vector<size_t>& organism_ids);
    
    /**
     * @brief Update world state (deplete energy, remove dead organisms)
     */
    void update();
    
    /**
     * @brief Get organism by ID
     * @param id Organism ID
     * @return Pointer to organism or nullptr if not found
     */
    Organism* getOrganism(size_t id);
    
    /**
     * @brief Get food by ID
     * @param id Food ID
     * @return Pointer to food or nullptr if not found
     */
    Food* getFood(size_t id);
    
    /**
     * @brief Get the world's random number generator
     * @return Reference to RNG
     */
    std::mt19937& getRNG() { return rng; }
};

#endif // BIOLOGICAL_SIM_H
