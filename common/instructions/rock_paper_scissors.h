#ifndef ROCK_PAPER_SCISSORS_H
#define ROCK_PAPER_SCISSORS_H

#include "../instruction.h"
#include <cstdint>

class RockPaperScissors : public Instruction
{
public:
    uint16_t m_choice_addr;      // Memory address containing choice (0=rock, 1=paper, 2=scissors)
    uint16_t m_opponent_addr;    // Memory address containing opponent ID
    uint16_t m_result_addr;      // Memory address to store result

    RockPaperScissors(uint16_t choice_addr, uint16_t opponent_addr, uint16_t result_addr);
    void operator()(std::vector<int32_t>& memory, uint8_t& subprogram_index,
        std::vector<uint16_t>& instruction_addresses, std::vector<uint8_t>& return_indices) const override;
    std::vector<std::string> toStringTokens() const override;
    std::vector<uint8_t> toByteArray() const override;
};

#endif // ROCK_PAPER_SCISSORS_H
