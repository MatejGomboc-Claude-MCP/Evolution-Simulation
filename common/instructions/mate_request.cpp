#include "mate_request.h"

MateRequest::MateRequest(uint16_t partner_index_addr, uint16_t result_addr)
    : m_partner_index_addr(partner_index_addr), m_result_addr(result_addr) {}

void MateRequest::operator()(std::vector<int32_t>& memory, uint8_t& subprogram_index,
    std::vector<uint16_t>& instruction_addresses, std::vector<uint8_t>& return_indices) const
{
    instruction_addresses[subprogram_index]++;
    
    // Get partner index from memory
    if (m_partner_index_addr < memory.size() && m_result_addr < memory.size()) {
        int32_t partner_index = memory[m_partner_index_addr];
        
        // In actual implementation, this would interface with the World class
        // For now, we just set a result flag in memory
        // 0 = request failed, 1 = request sent successfully
        memory[m_result_addr] = (partner_index >= 0) ? 1 : 0;
    }
}

std::vector<std::string> MateRequest::toStringTokens() const
{
    return {
        "MATE_REQUEST",
        std::to_string(m_partner_index_addr),
        std::to_string(m_result_addr)
    };
}

std::vector<uint8_t> MateRequest::toByteArray() const
{
    std::vector<uint8_t> result;
    result.push_back(static_cast<uint8_t>(Id::MATE_REQUEST));
    
    result.push_back(static_cast<uint8_t>(m_partner_index_addr));
    result.push_back(static_cast<uint8_t>(m_partner_index_addr >> 8));
    
    result.push_back(static_cast<uint8_t>(m_result_addr));
    result.push_back(static_cast<uint8_t>(m_result_addr >> 8));
    
    return result;
}
