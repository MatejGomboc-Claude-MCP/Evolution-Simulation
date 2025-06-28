#include "mate_accept.h"

MateAccept::MateAccept(uint16_t requester_index_addr, uint16_t result_addr)
    : m_requester_index_addr(requester_index_addr), m_result_addr(result_addr) {}

void MateAccept::operator()(std::vector<int32_t>& memory, uint8_t& subprogram_index,
    std::vector<uint16_t>& instruction_addresses, std::vector<uint8_t>& return_indices) const
{
    instruction_addresses[subprogram_index]++;
    
    // Get requester index from memory
    if (m_requester_index_addr < memory.size() && m_result_addr < memory.size()) {
        int32_t requester_index = memory[m_requester_index_addr];
        
        // In actual implementation, this would interface with the World class
        // For now, we just set a result flag in memory
        // 0 = acceptance failed, 1 = mating successful
        memory[m_result_addr] = (requester_index >= 0) ? 1 : 0;
    }
}

std::vector<std::string> MateAccept::toStringTokens() const
{
    return {
        "MATE_ACCEPT",
        std::to_string(m_requester_index_addr),
        std::to_string(m_result_addr)
    };
}

std::vector<uint8_t> MateAccept::toByteArray() const
{
    std::vector<uint8_t> result;
    result.push_back(static_cast<uint8_t>(Id::MATE_ACCEPT));
    
    result.push_back(static_cast<uint8_t>(m_requester_index_addr));
    result.push_back(static_cast<uint8_t>(m_requester_index_addr >> 8));
    
    result.push_back(static_cast<uint8_t>(m_result_addr));
    result.push_back(static_cast<uint8_t>(m_result_addr >> 8));
    
    return result;
}
