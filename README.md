# Evolution-Simulation

A genetic programming framework that simulates evolution through virtual machines executing instruction-based programs with biological features like energy, mating, and food consumption.

## Overview

Evolution-Simulation is a C++ framework for evolutionary computation where programs are represented as sequences of instructions that manipulate memory. These programs control organisms that must survive, find food, mate, and pass on their genetic information to the next generation.

### Features

- **Virtual Machine**: Execute programs consisting of various instruction types
- **Rich Instruction Set**: 26 instructions including arithmetic, logic, control flow, memory operations, and biological behaviors
- **Biological Simulation**:
  - Energy system with depletion over time
  - Mating and reproduction mechanics
  - Food consumption for survival
  - Conflict resolution through rock-paper-scissors and energy comparison
- **Subprogram Support**: Programs can contain multiple subprograms that can call each other
- **Memory System**: Programs have access to memory for computation and data storage
- **Serialization**: Save and load programs from text files
- **Random Program Generation**: Generate random programs for evolutionary algorithms

### Instruction Set

The framework supports the following instructions:

#### Basic Instructions
- **Arithmetic**: ADD, SUBTRACT, MULTIPLY, DIVIDE, INCREASE, DECREASE, NEGATE
- **Logic**: AND, OR, NOT, INVERT
- **Comparison**: EQUAL, GREATER, SMALLER
- **Memory**: INIT, COPY, SET, CLEAR
- **Control Flow**: CONDITION, LOOP, RETURN
- **Other**: NOP (no operation)

#### Biological Instructions
- **MATE_REQUEST**: Request mating with another organism (parameters: partner_index_addr, result_addr)
- **MATE_ACCEPT**: Accept a mating request (parameters: requester_index_addr, result_addr)
- **CONSUME_FOOD**: Attempt to consume food (parameters: food_index_addr, result_addr)
- **ROCK_PAPER_SCISSORS**: Play rock-paper-scissors for conflict resolution (parameters: choice_addr, opponent_addr, result_addr)

## Building

### Requirements

- CMake 3.0 or higher
- C++17 compatible compiler
- Standard C++ library

### Build Instructions

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### Running the Evolution Simulation

```bash
./evolution-sim/evolution-sim [population_size] [mutation_rate] [generations] [food_spawn_rate]

# Example with default parameters
./evolution-sim/evolution-sim

# Example with custom parameters
./evolution-sim/evolution-sim 100 0.15 200 30
```

Parameters:
- `population_size`: Initial number of organisms (default: 50)
- `mutation_rate`: Probability of mutation during reproduction (default: 0.2)
- `generations`: Number of generations to simulate (default: 100)
- `food_spawn_rate`: Number of food items spawned per generation (default: 20)

### Creating a Random Program

```cpp
#include "program.h"

// Generate a random program
Program program = Program::random();

// Execute the program
program.execute();
```

### Loading a Program from File

```cpp
#include "txtfileparser.h"

std::vector<std::unique_ptr<Instruction>> subprogram;
size_t status = TxtFileParser::subprogramFromTxt(file_content, subprogram);
if (status == TxtFileParser::STATUS_SUCCESS) {
    // Program loaded successfully
}
```

### Program File Format

Programs are stored in text files with instructions on separate lines. Example:

```
INIT 0 100
INIT 1 50
MATE_REQUEST 0 2
CONDITION 2 5
CONSUME_FOOD 1 3
RETURN
```

### Biological Simulation Details

#### Energy System
- Organisms start with 100 energy units
- Energy depletes by 1 unit per time step
- Organisms die when energy reaches 0
- Food provides 30 energy units when consumed
- Mating costs 20 energy units for each parent

#### Conflict Resolution
When organisms compete, the winner is determined by:
1. Rock-Paper-Scissors game (based on organism's choice)
2. If tied, higher energy wins
3. If still tied, random selection (50/50 chance)

## Project Structure

- `common/` - Core library with instruction and program implementations
  - `instruction.h/cpp` - Base instruction class
  - `program.h/cpp` - Program execution engine
  - `biological_sim.h/cpp` - Biological simulation components
  - `instructions/` - Individual instruction implementations
  - `txtfileparser.h/cpp` - File I/O for programs
  - `utils.h/cpp` - Utility functions
- `evolution-sim/` - Main evolution simulation program
- `test-save-load/` - Test program for file I/O functionality
- `examples/` - Example program files

## Examples

See the `examples/` directory for sample programs demonstrating various features:
- `simple_math.txt` - Basic arithmetic operations
- `loop_example.txt` - Loop control flow
- `condition_example.txt` - Conditional execution
- `logic_operations.txt` - Logical operations

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## License

Copyright (C) 2021 Matej Gomboc

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
