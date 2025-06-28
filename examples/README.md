# Example Programs

This directory contains example programs that can be loaded and executed by the Evolution Simulation framework.

## File Format

Programs are stored as text files with one instruction per line. Each instruction consists of:
- Instruction name (uppercase)
- Parameters (space-separated)

## Examples

- `simple_math.txt` - Basic arithmetic operations
- `loop_example.txt` - Demonstrates loop control flow
- `condition_example.txt` - Shows conditional execution
- `subprogram_example.txt` - Example with multiple subprograms

## Loading Programs

To load a program from a text file:

```cpp
#include "txtfileparser.h"

std::vector<std::unique_ptr<Instruction>> subprogram;
size_t status = TxtFileParser::subprogramFromTxt(file_content, subprogram);
```

## Creating Your Own Programs

You can create your own program files following the instruction format. See the instruction set documentation in the main README for available instructions and their parameters.
