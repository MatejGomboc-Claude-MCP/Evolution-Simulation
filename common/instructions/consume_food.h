#ifndef CONSUME_FOOD_H
#define CONSUME_FOOD_H

#include "../instruction.h"
#include <cstdint>

class ConsumeFood : public Instruction
{
public:
    uint16_t m_food_index_addr;
    uint16_t m_result_addr;

    ConsumeFood(uint16_t food_index_addr, uint16_t result_addr);
    void operator()(std::vector<int32_t>& memory, uint8_t& subprogram_index,
        std::vector<uint16_t>& instruction_addresses, std::vector<uint8_t>& return_indices) const override;
    std::vector<std::string> toStringTokens() const override;
    std::vector<uint8_t> toByteArray() const override;
};

#endif // CONSUME_FOOD_H
