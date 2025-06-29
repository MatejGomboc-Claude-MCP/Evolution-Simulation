#ifndef MATE_ACCEPT_H
#define MATE_ACCEPT_H

#include "../instruction.h"
#include <cstdint>

class MateAccept : public Instruction
{
public:
    uint16_t m_mate_index_addr;

    MateAccept(uint16_t mate_index_addr);
    void operator()(std::vector<int32_t>& memory, uint8_t& subprogram_index,
        std::vector<uint16_t>& instruction_addresses, std::vector<uint8_t>& return_indices) const override;
    std::vector<std::string> toStringTokens() const override;
    std::vector<uint8_t> toByteArray() const override;
};

#endif // MATE_ACCEPT_H
