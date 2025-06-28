#ifndef PROGRAM_H
#define PROGRAM_H

#include <limits>
#include "instruction.h"

/**
 * @class Program
 * @brief Represents an executable program consisting of instructions organized in subprograms
 * 
 * A Program is the main execution unit in the evolution simulation. It contains:
 * - Multiple subprograms (collections of instructions)
 * - Memory space for data storage and manipulation
 * - Execution state (instruction pointers, return addresses)
 * 
 * Programs can be randomly generated for evolutionary algorithms or loaded from files.
 */
class Program
{
public:
    /// Maximum number of instructions per subprogram
    static constexpr uint16_t MAX_INSTRUCTIONS = std::numeric_limits<uint16_t>::max();
    
    /// Maximum size of program memory
    static constexpr uint16_t MAX_DATA = std::numeric_limits<uint16_t>::max();
    
    /// Maximum number of subprograms in a program
    static constexpr uint8_t MAX_SUBPROGRAMS = std::numeric_limits<uint8_t>::max();

    /**
     * @brief Generate a random program
     * @return A newly created program with random instructions
     */
    static Program random();

    /**
     * @brief Default constructor
     */
    Program();
    
    /**
     * @brief Execute the program
     * 
     * Starts execution from subprogram 0 and continues until a RETURN
     * instruction is encountered or an error occurs.
     */
    void execute();

private:
    /// Collection of subprograms, each containing a sequence of instructions
    std::vector<std::vector<std::unique_ptr<Instruction>>> m_subprograms;
    
    /// Program memory for data storage
    std::vector<int32_t> m_memory;
    
    /// Current subprogram being executed
    uint8_t m_subprogram_index = 0;
    
    /// Stack of instruction addresses for each subprogram
    std::vector<uint16_t> m_instruction_addresses;
    
    /// Stack of return addresses for subprogram calls
    std::vector<uint8_t> m_return_addresses;

    /**
     * @brief Generate a random memory value
     * @return Random 32-bit integer value
     */
    static int32_t generateRandomMemoryValue();

    /**
     * @brief Generate a random memory address
     * @param adding_allowed Whether to allow adding new memory locations
     * @param max_allowed_memory Maximum memory address allowed
     * @return Random memory address within bounds
     */
    uint16_t generateRandomMemoryAddress(bool adding_allowed = false, uint16_t max_allowed_memory = 0);
    
    /**
     * @brief Add a NOP (no operation) instruction to a subprogram
     * @param subprogram_index Target subprogram
     */
    void addNop(uint8_t subprogram_index);
    
    /**
     * @brief Add a random INIT instruction to a subprogram
     * @param subprogram_index Target subprogram
     * @param adding_memory_allowed Whether to allow expanding memory
     * @param max_allowed_memory Maximum memory address allowed
     */
    void addRandomInit(uint8_t subprogram_index, bool adding_memory_allowed = false, uint16_t max_allowed_memory = 0);
    
    /**
     * @brief Add a random COPY instruction to a subprogram
     * @param subprogram_index Target subprogram
     * @param adding_memory_allowed Whether to allow expanding memory
     * @param max_allowed_memory Maximum memory address allowed
     */
    void addRandomCopy(uint8_t subprogram_index, bool adding_memory_allowed = false, uint16_t max_allowed_memory = 0);
    
    /**
     * @brief Add a random unary math/logic instruction to a subprogram
     * @param subprogram_index Target subprogram
     * @param adding_memory_allowed Whether to allow expanding memory
     * @param max_allowed_memory Maximum memory address allowed
     */
    void addRandomMathLogicInstruction1(uint8_t subprogram_index, bool adding_memory_allowed = false, uint16_t max_allowed_memory = 0);
    
    /**
     * @brief Add a random binary math/logic instruction to a subprogram
     * @param subprogram_index Target subprogram
     * @param adding_memory_allowed Whether to allow expanding memory
     * @param max_allowed_memory Maximum memory address allowed
     */
    void addRandomMathLogicInstruction2(uint8_t subprogram_index, bool adding_memory_allowed = false, uint16_t max_allowed_memory = 0);
};

#endif // PROGRAM_H
