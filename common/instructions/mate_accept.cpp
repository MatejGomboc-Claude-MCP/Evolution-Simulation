#include "mate_accept.h"

MateAccept::MateAccept(uint16_t mate_index_addr)
    : m_mate_index_addr(mate_index_addr) {}

void MateAccept::operator()(std::vector<int32_t>& memory, uint8_t& subprogram_index,
    std::vector<uint16_t>& instruction_addresses, std::vector<uint8_t>& return_indices) const
{
    instruction_addresses[subprogram_index]++;
    
    // Get mate index from memory
    if (m_mate_index_addr < memory.size()) {
        int32_t mate_index = memory[m_mate_index_addr];
        
        // In actual implementation, this would interface with the World class
        // The organism's brain should have already decided whether to accept
        // based on the mate's fitness or other factors using arithmetic instructions
        // This instruction just executes the acceptance
    }
}

std::vector<std::string> MateAccept::toStringTokens() const
{
    return {
        "MATE_ACCEPT",
        std::to_string(m_mate_index_addr)
    };
}

std::vector<uint8_t> MateAccept::toByteArray() const
{
    std::vector<uint8_t> result;
    result.push_back(static_cast<uint8_t>(Id::MATE_ACCEPT));
    
    result.push_back(static_cast<uint8_t>(m_mate_index_addr));
    result.push_back(static_cast<uint8_t>(m_mate_index_addr >> 8));
    
    return result;
}
