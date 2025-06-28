#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>
#include <memory>
#include <string>
#include <cstdint>

class Instruction
{
public:
    enum class Id : uint8_t {
        ADD = 0,
        AND = 1,
        CLEAR = 2,
        CONDITION = 3,
        COPY = 4,
        DECREASE = 5,
        DIVIDE = 6,
        EQUAL = 7,
        GREATER = 8,
        INCREASE = 9,
        INIT = 10,
        INVERT = 11,
        LOOP = 12,
        MULTIPLY = 13,
        NEGATE = 14,
        NOP = 15,
        NOT = 16,
        OR = 17,
        RETURN = 18,
        SET = 19,
        SMALLER = 20,
        SUBTRACT = 21,
        // New biological simulation instructions
        MATE_REQUEST = 22,    // Request mating with another organism
        CONSUME_FOOD = 23,    // Attempt to consume food
        MATE_ACCEPT = 24,     // Accept mating request from another organism
        ROCK_PAPER_SCISSORS = 25  // Play rock-paper-scissors (for conflict resolution)
    };

    virtual ~Instruction() = 0;
    static std::unique_ptr<Instruction> fromStringTokens(const std::vector<std::string>& tokens);
    static std::unique_ptr<Instruction> fromByteArray(const std::vector<uint8_t>& array, size_t& offset);
    virtual void operator()(std::vector<int32_t>& memory, uint8_t& subprogram_index,
        std::vector<uint16_t>& instruction_addresses, std::vector<uint8_t>& return_indices) const = 0;
    virtual std::vector<std::string> toStringTokens() const = 0;
    virtual std::vector<uint8_t> toByteArray() const = 0;
};

#endif // INSTRUCTION_H
