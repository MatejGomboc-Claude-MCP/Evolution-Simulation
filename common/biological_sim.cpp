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

void Organism::mutate(std::mt19937& rng) {
    std::uniform_real_distribution<double> mut_dist(0.0, 1.0);
    
    // For simplicity, we'll regenerate parts of the program
    // In a real implementation, you would mutate individual instructions
    if (mut_dist(rng) < MUTATION_RATE) {
        // Generate a new random program (heavy mutation)
        program = Program::random();
    }
    // You could add more sophisticated mutation operators here:
    // - Change individual instruction parameters
    // - Add/remove instructions
    // - Swap instruction order
    // - Modify memory addresses
}

// Food implementation
Food::Food(size_t food_id, std::mt19937* rng, double energy) 
    : id(food_id), energy_value(energy), consumed(false), world_rng(rng) {}

double Food::consume() {
    if (!consumed) {
        consumed = true;
        return energy_value;
    }
    return 0.0;
}

RPSChoice Food::makeRPSChoice(const Organism* organism) const {
    // Use organism's internal state to make a choice
    int choice = static_cast<int>(organism->id + organism->age) % 3;
    return static_cast<RPSChoice>(choice);
}

RPSResult Food::playRockPaperScissors(RPSChoice player1, RPSChoice player2) {
    if (player1 == player2) return RPSResult::TIE;
    
    if ((player1 == RPSChoice::ROCK && player2 == RPSChoice::SCISSORS) ||
        (player1 == RPSChoice::PAPER && player2 == RPSChoice::ROCK) ||
        (player1 == RPSChoice::SCISSORS && player2 == RPSChoice::PAPER)) {
        return RPSResult::PLAYER1_WINS;
    }
    
    return RPSResult::PLAYER2_WINS;
}

size_t Food::resolveConsumptionConflict(const std::vector<Organism*>& organisms) {
    if (organisms.empty() || consumed) return -1;
    if (organisms.size() == 1) return organisms[0]->id;
    
    // Multiple organisms competing - use the conflict resolution algorithm
    std::vector<Organism*> candidates = organisms;
    
    // First round: Rock-Paper-Scissors elimination
    while (candidates.size() > 1) {
        std::vector<Organism*> winners;
        
        // Pair up organisms for RPS games
        for (size_t i = 0; i + 1 < candidates.size(); i += 2) {
            RPSChoice choice1 = makeRPSChoice(candidates[i]);
            RPSChoice choice2 = makeRPSChoice(candidates[i + 1]);
            RPSResult result = playRockPaperScissors(choice1, choice2);
            
            if (result == RPSResult::PLAYER1_WINS) {
                winners.push_back(candidates[i]);
            } else if (result == RPSResult::PLAYER2_WINS) {
                winners.push_back(candidates[i + 1]);
            } else {
                // Tie: use energy as tiebreaker
                if (candidates[i]->energy > candidates[i + 1]->energy) {
                    winners.push_back(candidates[i]);
                } else if (candidates[i + 1]->energy > candidates[i]->energy) {
                    winners.push_back(candidates[i + 1]);
                } else {
                    // Still tied: random selection
                    std::bernoulli_distribution dist(0.5);
                    winners.push_back(dist(*world_rng) ? candidates[i] : candidates[i + 1]);
                }
            }
        }
        
        // If odd number, last organism advances automatically
        if (candidates.size() % 2 == 1) {
            winners.push_back(candidates.back());
        }
        
        candidates = winners;
    }
    
    return candidates[0]->id;
}

// World implementation
World::World() : next_organism_id(0), next_food_id(0) {
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    rng.seed(seed);
}

Organism* World::addOrganism(bool apply_initial_mutations) {
    auto organism = std::make_unique<Organism>(next_organism_id++);
    
    // Apply initial mutations if requested
    if (apply_initial_mutations) {
        organism->mutate(rng);
    }
    
    Organism* ptr = organism.get();
    organisms.push_back(std::move(organism));
    return ptr;
}

void World::addFood(size_t count) {
    for (size_t i = 0; i < count; ++i) {
        food_items.push_back(std::make_unique<Food>(next_food_id++, &rng));
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
    
    // Create offspring (without initial mutations - they'll be applied during creation)
    Organism* child = addOrganism(false);
    
    // Simple genetic combination: randomly choose instructions from parents
    std::uniform_int_distribution<int> dist(0, 1);
    
    // Inherit some fitness from parents (with variation)
    std::normal_distribution<double> fitness_dist(
        (acceptor->fitness + requester->fitness) / 2.0, 0.1);
    child->fitness = std::max(0.0, fitness_dist(rng));
    
    // Give child some initial energy from parents
    child->energy = (acceptor->energy + requester->energy) * 0.1 + Organism::INITIAL_ENERGY * 0.5;
    
    // TODO: Implement proper genetic crossover between parent programs
    // For now, randomly choose one parent's program
    if (dist(rng) == 0) {
        child->program = acceptor->program;
    } else {
        child->program = requester->program;
    }
    
    // Apply mutations to the child
    child->mutate(rng);
    
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
    
    if (!organism || !food || food->consumed) return false;
    
    // For single organism, just consume
    double energy_gained = food->consume();
    if (energy_gained > 0) {
        organism->gainEnergy(energy_gained);
        return true;
    }
    
    return false;
}

void World::resolveMultipleFoodConsumption(size_t food_id, const std::vector<size_t>& organism_ids) {
    Food* food = getFood(food_id);
    if (!food || food->consumed) return;
    
    std::vector<Organism*> competing_organisms;
    for (size_t id : organism_ids) {
        Organism* org = getOrganism(id);
        if (org) {
            competing_organisms.push_back(org);
        }
    }
    
    size_t winner_id = food->resolveConsumptionConflict(competing_organisms);
    if (winner_id != static_cast<size_t>(-1)) {
        consumeFood(winner_id, food_id);
    }
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
