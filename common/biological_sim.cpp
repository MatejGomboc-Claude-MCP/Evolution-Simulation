#include "biological_sim.h"
#include <algorithm>
#include <chrono>

// Organism implementation
Organism::Organism(size_t organism_id) 
    : fitness(0.0), energy(INITIAL_ENERGY), id(organism_id), age(0) {
    program = Program::random();
}

bool Organism::depleteEnergy() {
    energy -= ENERGY_DEPLETION_RATE;
    age++;
    return energy > 0;
}

bool Organism::hasEnergy(double required_energy) const {
    return energy >= required_energy;
}

void Organism::consumeEnergy(double amount) {
    energy = std::max(0.0, energy - amount);
}

void Organism::gainEnergy(double amount) {
    energy += amount;
}

RPSChoice Organism::makeRPSChoice() const {
    // Use organism's internal state to make a choice
    // This could be based on memory values or other factors
    int choice = static_cast<int>(id + age) % 3;
    return static_cast<RPSChoice>(choice);
}

// Food implementation
Food::Food(size_t food_id, double energy) 
    : id(food_id), energy_value(energy), consumed(false) {}

double Food::consume() {
    if (!consumed) {
        consumed = true;
        return energy_value;
    }
    return 0.0;
}

// World implementation
World::World() : next_organism_id(0), next_food_id(0) {
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng.seed(seed);
}

Organism* World::addOrganism() {
    auto organism = std::make_unique<Organism>(next_organism_id++);
    Organism* ptr = organism.get();
    organisms.push_back(std::move(organism));
    return ptr;
}

void World::addFood(size_t count) {
    for (size_t i = 0; i < count; ++i) {
        food_items.push_back(std::make_unique<Food>(next_food_id++));
    }
}

bool World::processMatingRequest(size_t requester_id, size_t target_id) {
    Organism* requester = getOrganism(requester_id);
    Organism* target = getOrganism(target_id);
    
    if (!requester || !target) return false;
    if (!requester->hasEnergy(Organism::MATING_ENERGY_COST)) return false;
    if (requester_id == target_id) return false; // Can't mate with self
    
    // Register the mating request
    requester->pending_mating_request = MatingRequest{requester_id, target_id, false};
    target->mating_requests_received.push_back(requester_id);
    
    return true;
}

Organism* World::processMatingAcceptance(size_t acceptor_id, size_t requester_id) {
    Organism* acceptor = getOrganism(acceptor_id);
    Organism* requester = getOrganism(requester_id);
    
    if (!acceptor || !requester) return nullptr;
    
    // Check if the requester actually made a request to this acceptor
    if (!requester->pending_mating_request.has_value() ||
        requester->pending_mating_request->target_id != acceptor_id) {
        return nullptr;
    }
    
    // Check energy requirements
    if (!acceptor->hasEnergy(Organism::MATING_ENERGY_COST) ||
        !requester->hasEnergy(Organism::MATING_ENERGY_COST)) {
        return nullptr;
    }
    
    // Consume energy from both parents
    acceptor->consumeEnergy(Organism::MATING_ENERGY_COST);
    requester->consumeEnergy(Organism::MATING_ENERGY_COST);
    
    // Create offspring
    Organism* child = addOrganism();
    
    // Simple genetic combination: randomly choose instructions from parents
    // In a more sophisticated implementation, this would involve crossover and mutation
    std::uniform_int_distribution<int> dist(0, 1);
    
    // Inherit some fitness from parents (with variation)
    std::normal_distribution<double> fitness_dist(
        (acceptor->fitness + requester->fitness) / 2.0, 0.1);
    child->fitness = std::max(0.0, fitness_dist(rng));
    
    // Give child some initial energy from parents
    child->energy = (acceptor->energy + requester->energy) * 0.1 + Organism::INITIAL_ENERGY * 0.5;
    
    // Clear mating request
    requester->pending_mating_request.reset();
    
    // Remove request from acceptor's list
    auto& requests = acceptor->mating_requests_received;
    requests.erase(std::remove(requests.begin(), requests.end(), requester_id), requests.end());
    
    return child;
}

bool World::consumeFood(size_t organism_id, size_t food_id) {
    Organism* organism = getOrganism(organism_id);
    Food* food = getFood(food_id);
    
    if (!organism || !food) return false;
    
    double energy_gained = food->consume();
    if (energy_gained > 0) {
        organism->gainEnergy(energy_gained);
        return true;
    }
    
    return false;
}

RPSResult World::playRockPaperScissors(RPSChoice player1, RPSChoice player2) {
    if (player1 == player2) return RPSResult::TIE;
    
    if ((player1 == RPSChoice::ROCK && player2 == RPSChoice::SCISSORS) ||
        (player1 == RPSChoice::PAPER && player2 == RPSChoice::ROCK) ||
        (player1 == RPSChoice::SCISSORS && player2 == RPSChoice::PAPER)) {
        return RPSResult::PLAYER1_WINS;
    }
    
    return RPSResult::PLAYER2_WINS;
}

size_t World::resolveConflict(size_t org1_id, size_t org2_id) {
    Organism* org1 = getOrganism(org1_id);
    Organism* org2 = getOrganism(org2_id);
    
    if (!org1 || !org2) return org1 ? org1_id : org2_id;
    
    // First: Rock-Paper-Scissors
    RPSChoice choice1 = org1->makeRPSChoice();
    RPSChoice choice2 = org2->makeRPSChoice();
    RPSResult rps_result = playRockPaperScissors(choice1, choice2);
    
    if (rps_result == RPSResult::PLAYER1_WINS) return org1_id;
    if (rps_result == RPSResult::PLAYER2_WINS) return org2_id;
    
    // Tie: Use energy as indicator of dominance
    if (org1->energy > org2->energy) return org1_id;
    if (org2->energy > org1->energy) return org2_id;
    
    // Still tied: Random selection (Bernoulli distribution)
    std::bernoulli_distribution dist(0.5);
    return dist(rng) ? org1_id : org2_id;
}

void World::update() {
    // Remove dead organisms
    organisms.erase(
        std::remove_if(organisms.begin(), organisms.end(),
            [](const std::unique_ptr<Organism>& org) {
                return !const_cast<Organism*>(org.get())->depleteEnergy();
            }),
        organisms.end()
    );
    
    // Remove consumed food
    food_items.erase(
        std::remove_if(food_items.begin(), food_items.end(),
            [](const std::unique_ptr<Food>& food) {
                return food->consumed;
            }),
        food_items.end()
    );
}

Organism* World::getOrganism(size_t id) {
    auto it = std::find_if(organisms.begin(), organisms.end(),
        [id](const std::unique_ptr<Organism>& org) {
            return org->id == id;
        });
    
    return (it != organisms.end()) ? it->get() : nullptr;
}

Food* World::getFood(size_t id) {
    auto it = std::find_if(food_items.begin(), food_items.end(),
        [id](const std::unique_ptr<Food>& food) {
            return food->id == id;
        });
    
    return (it != food_items.end()) ? it->get() : nullptr;
}
