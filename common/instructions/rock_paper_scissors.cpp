#include "rock_paper_scissors.h"

RockPaperScissors::RockPaperScissors(uint16_t choice_addr, uint16_t opponent_addr, uint16_t result_addr)
    : m_choice_addr(choice_addr), m_opponent_addr(opponent_addr), m_result_addr(result_addr) {}

void RockPaperScissors::operator()(std::vector<int32_t>& memory, uint8_t& subprogram_index,
    std::vector<uint16_t>& instruction_addresses, std::vector<uint8_t>& return_indices) const
{
    instruction_addresses[subprogram_index]++;
    
    // Get choice and opponent from memory
    if (m_choice_addr < memory.size() && 
        m_opponent_addr < memory.size() && 
        m_result_addr < memory.size()) {
        
        int32_t choice = memory[m_choice_addr] % 3;  // Ensure valid choice (0-2)
        int32_t opponent_id = memory[m_opponent_addr];
        
        // In actual implementation, this would interface with the World class
        // to resolve the conflict. For now, we just store a placeholder result
        // -1 = loss, 0 = tie, 1 = win
        memory[m_result_addr] = 0;  // Default to tie
    }
}

std::vector<std::string> RockPaperScissors::toStringTokens() const
{
    return {
        "ROCK_PAPER_SCISSORS",
        std::to_string(m_choice_addr),
        std::to_string(m_opponent_addr),
        std::to_string(m_result_addr)
    };
}

std::vector<uint8_t> RockPaperScissors::toByteArray() const
{
    std::vector<uint8_t> result;
    result.push_back(static_cast<uint8_t>(Id::ROCK_PAPER_SCISSORS));
    
    result.push_back(static_cast<uint8_t>(m_choice_addr));
    result.push_back(static_cast<uint8_t>(m_choice_addr >> 8));
    
    result.push_back(static_cast<uint8_t>(m_opponent_addr));
    result.push_back(static_cast<uint8_t>(m_opponent_addr >> 8));
    
    result.push_back(static_cast<uint8_t>(m_result_addr));
    result.push_back(static_cast<uint8_t>(m_result_addr >> 8));
    
    return result;
}
