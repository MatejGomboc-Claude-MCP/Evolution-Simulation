#ifndef MATE_REQUEST_H
#define MATE_REQUEST_H

#include "../instruction.h"
#include <cstdint>

class MateRequest : public Instruction
{
public:
    uint16_t m_partner_index_addr;
    uint16_t m_result_addr;

    MateRequest(uint16_t partner_index_addr, uint16_t result_addr);
    void operator()(std::vector<int32_t>& memory, uint8_t& subprogram_index,
        std::vector<uint16_t>& instruction_addresses, std::vector<uint8_t>& return_indices) const override;
    std::vector<std::string> toStringTokens() const override;
    std::vector<uint8_t> toByteArray() const override;
};

#endif // MATE_REQUEST_H
