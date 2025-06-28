#include "consume_food.h"

ConsumeFood::ConsumeFood(uint16_t food_index_addr, uint16_t result_addr)
    : m_food_index_addr(food_index_addr), m_result_addr(result_addr) {}

void ConsumeFood::operator()(std::vector<int32_t>& memory, uint8_t& subprogram_index,
    std::vector<uint16_t>& instruction_addresses, std::vector<uint8_t>& return_indices) const
{
    instruction_addresses[subprogram_index]++;
    
    // Get food index from memory
    if (m_food_index_addr < memory.size() && m_result_addr < memory.size()) {
        int32_t food_index = memory[m_food_index_addr];
        
        // In actual implementation, this would interface with the World class
        // For now, we just set a result flag in memory
        // 0 = consumption failed, 1 = consumption successful
        memory[m_result_addr] = (food_index >= 0) ? 1 : 0;
    }
}

std::vector<std::string> ConsumeFood::toStringTokens() const
{
    return {
        "CONSUME_FOOD",
        std::to_string(m_food_index_addr),
        std::to_string(m_result_addr)
    };
}

std::vector<uint8_t> ConsumeFood::toByteArray() const
{
    std::vector<uint8_t> result;
    result.push_back(static_cast<uint8_t>(Id::CONSUME_FOOD));
    
    result.push_back(static_cast<uint8_t>(m_food_index_addr));
    result.push_back(static_cast<uint8_t>(m_food_index_addr >> 8));
    
    result.push_back(static_cast<uint8_t>(m_result_addr));
    result.push_back(static_cast<uint8_t>(m_result_addr >> 8));
    
    return result;
}
