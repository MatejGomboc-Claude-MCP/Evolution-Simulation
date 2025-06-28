# Evolution-Simulation

A genetic programming framework that simulates evolution through virtual machines executing instruction-based programs.

## Overview

Evolution-Simulation is a C++ framework for evolutionary computation where programs are represented as sequences of instructions that manipulate memory. These programs can evolve over time through mutation and selection, allowing complex behaviors to emerge.

### Features

- **Virtual Machine**: Execute programs consisting of various instruction types
- **Instruction Set**: Rich set of 22 instructions including arithmetic, logic, control flow, and memory operations
- **Subprogram Support**: Programs can contain multiple subprograms that can call each other
- **Memory System**: Programs have access to memory for computation and data storage
- **Serialization**: Save and load programs from text files
- **Random Program Generation**: Generate random programs for evolutionary algorithms

### Instruction Set

The framework supports the following instructions:

- **Arithmetic**: ADD, SUBTRACT, MULTIPLY, DIVIDE, INCREASE, DECREASE, NEGATE
- **Logic**: AND, OR, NOT, INVERT
- **Comparison**: EQUAL, GREATER, SMALLER
- **Memory**: INIT, COPY, SET, CLEAR
- **Control Flow**: CONDITION, LOOP, RETURN
- **Other**: NOP (no operation)

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
COPY 0 1
ADD 0 1 2
CONDITION 2 5
LOOP 3 10
RETURN
```

## Project Structure

- `common/` - Core library with instruction and program implementations
  - `instruction.h/cpp` - Base instruction class
  - `program.h/cpp` - Program execution engine
  - `instructions/` - Individual instruction implementations
  - `txtfileparser.h/cpp` - File I/O for programs
  - `utils.h/cpp` - Utility functions
- `test-save-load/` - Test program for file I/O functionality

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
