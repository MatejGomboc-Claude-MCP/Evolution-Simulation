#include <limits>
#include <unordered_map>
#include "instruction.h"
#include "add.h"
#include "and.h"
#include "clear.h"
#include "condition.h"
#include "copy.h"
#include "decrease.h"
#include "divide.h"
#include "equal.h"
#include "greater.h"
#include "increase.h"
#include "init.h"
#include "invert.h"
#include "loop.h"
#include "multiply.h"
#include "negate.h"
#include "nop.h"
#include "not.h"
#include "or_.h"
#include "return.h"
#include "set.h"
#include "smaller.h"
#include "subtract.h"
#include "mate_request.h"
#include "consume_food.h"
#include "mate_accept.h"
#include "rock_paper_scissors.h"

Instruction::~Instruction() = default;

std::unique_ptr<Instruction> Instruction::fromStringTokens(const std::vector<std::string>& tokens)
{
    static const std::unordered_map<std::string,
        std::unique_ptr<Instruction>(*)(const std::vector<std::string>& tokens)> FACTORIES_TABLE = {
        { Add::MNEMONIC, Add::fromStringTokens },
        { And::MNEMONIC, And::fromStringTokens },
        { Clear::MNEMONIC, Clear::fromStringTokens },
        { Condition::MNEMONIC, Condition::fromStringTokens },
        { Copy::MNEMONIC, Copy::fromStringTokens },
        { Decrease::MNEMONIC, Decrease::fromStringTokens },
        { Divide::MNEMONIC, Divide::fromStringTokens },
        { Equal::MNEMONIC, Equal::fromStringTokens },
        { Greater::MNEMONIC, Greater::fromStringTokens },
        { Increase::MNEMONIC, Increase::fromStringTokens },
        { Init::MNEMONIC, Init::fromStringTokens },
        { Invert::MNEMONIC, Invert::fromStringTokens },
        { Loop::MNEMONIC, Loop::fromStringTokens },
        { Multiply::MNEMONIC, Multiply::fromStringTokens },
        { Negate::MNEMONIC, Negate::fromStringTokens },
        { Nop::MNEMONIC, Nop::fromStringTokens },
        { Not::MNEMONIC, Not::fromStringTokens },
        { Or::MNEMONIC, Or::fromStringTokens },
        { Return::MNEMONIC, Return::fromStringTokens },
        { Set::MNEMONIC, Set::fromStringTokens },
        { Smaller::MNEMONIC, Smaller::fromStringTokens },
        { Subtract::MNEMONIC, Subtract::fromStringTokens },
        { "MATE_REQUEST", [](const std::vector<std::string>& tokens) -> std::unique_ptr<Instruction> {
            if (tokens.size() != 3) return nullptr;
            try {
                uint16_t partner_addr = std::stoi(tokens[1]);
                uint16_t result_addr = std::stoi(tokens[2]);
                return std::make_unique<MateRequest>(partner_addr, result_addr);
            } catch (...) {
                return nullptr;
            }
        }},
        { "CONSUME_FOOD", [](const std::vector<std::string>& tokens) -> std::unique_ptr<Instruction> {
            if (tokens.size() != 3) return nullptr;
            try {
                uint16_t food_addr = std::stoi(tokens[1]);
                uint16_t result_addr = std::stoi(tokens[2]);
                return std::make_unique<ConsumeFood>(food_addr, result_addr);
            } catch (...) {
                return nullptr;
            }
        }},
        { "MATE_ACCEPT", [](const std::vector<std::string>& tokens) -> std::unique_ptr<Instruction> {
            if (tokens.size() != 3) return nullptr;
            try {
                uint16_t requester_addr = std::stoi(tokens[1]);
                uint16_t result_addr = std::stoi(tokens[2]);
                return std::make_unique<MateAccept>(requester_addr, result_addr);
            } catch (...) {
                return nullptr;
            }
        }},
        { "ROCK_PAPER_SCISSORS", [](const std::vector<std::string>& tokens) -> std::unique_ptr<Instruction> {
            if (tokens.size() != 4) return nullptr;
            try {
                uint16_t choice_addr = std::stoi(tokens[1]);
                uint16_t opponent_addr = std::stoi(tokens[2]);
                uint16_t result_addr = std::stoi(tokens[3]);
                return std::make_unique<RockPaperScissors>(choice_addr, opponent_addr, result_addr);
            } catch (...) {
                return nullptr;
            }
        }}
    };

    if (tokens.size() < 1) {
        return nullptr;
    }

    auto it = FACTORIES_TABLE.find(tokens[0]);
    if (it == FACTORIES_TABLE.end()) {
        return nullptr;
    }

    return it->second(tokens);
}

std::unique_ptr<Instruction> Instruction::fromByteArray(const std::vector<uint8_t>& array, size_t& offset)
{
    static const std::unordered_map<uint8_t,
        std::unique_ptr<Instruction>(*)(const std::vector<uint8_t>& array, size_t& offset)> FACTORIES_TABLE = {
        { static_cast<uint8_t>(Id::ADD), Add::fromByteArray },
        { static_cast<uint8_t>(Id::AND), And::fromByteArray },
        { static_cast<uint8_t>(Id::CLEAR), Clear::fromByteArray },
        { static_cast<uint8_t>(Id::CONDITION), Condition::fromByteArray },
        { static_cast<uint8_t>(Id::COPY), Copy::fromByteArray },
        { static_cast<uint8_t>(Id::DECREASE), Decrease::fromByteArray },
        { static_cast<uint8_t>(Id::DIVIDE), Divide::fromByteArray },
        { static_cast<uint8_t>(Id::EQUAL), Equal::fromByteArray },
        { static_cast<uint8_t>(Id::GREATER), Greater::fromByteArray },
        { static_cast<uint8_t>(Id::INCREASE), Increase::fromByteArray },
        { static_cast<uint8_t>(Id::INIT), Init::fromByteArray },
        { static_cast<uint8_t>(Id::INVERT), Invert::fromByteArray },
        { static_cast<uint8_t>(Id::LOOP), Loop::fromByteArray },
        { static_cast<uint8_t>(Id::MULTIPLY), Multiply::fromByteArray },
        { static_cast<uint8_t>(Id::NEGATE), Negate::fromByteArray },
        { static_cast<uint8_t>(Id::NOP), Nop::fromByteArray },
        { static_cast<uint8_t>(Id::NOT), Not::fromByteArray },
        { static_cast<uint8_t>(Id::OR), Or::fromByteArray },
        { static_cast<uint8_t>(Id::RETURN), Return::fromByteArray },
        { static_cast<uint8_t>(Id::SET), Set::fromByteArray },
        { static_cast<uint8_t>(Id::SMALLER), Smaller::fromByteArray },
        { static_cast<uint8_t>(Id::SUBTRACT), Subtract::fromByteArray },
        { static_cast<uint8_t>(Id::MATE_REQUEST), [](const std::vector<uint8_t>& array, size_t& offset) -> std::unique_ptr<Instruction> {
            if (array.size() - offset < 5) return nullptr;
            offset++;
            uint16_t partner_addr = array[offset] | (array[offset + 1] << 8);
            offset += 2;
            uint16_t result_addr = array[offset] | (array[offset + 1] << 8);
            offset += 2;
            return std::make_unique<MateRequest>(partner_addr, result_addr);
        }},
        { static_cast<uint8_t>(Id::CONSUME_FOOD), [](const std::vector<uint8_t>& array, size_t& offset) -> std::unique_ptr<Instruction> {
            if (array.size() - offset < 5) return nullptr;
            offset++;
            uint16_t food_addr = array[offset] | (array[offset + 1] << 8);
            offset += 2;
            uint16_t result_addr = array[offset] | (array[offset + 1] << 8);
            offset += 2;
            return std::make_unique<ConsumeFood>(food_addr, result_addr);
        }},
        { static_cast<uint8_t>(Id::MATE_ACCEPT), [](const std::vector<uint8_t>& array, size_t& offset) -> std::unique_ptr<Instruction> {
            if (array.size() - offset < 5) return nullptr;
            offset++;
            uint16_t requester_addr = array[offset] | (array[offset + 1] << 8);
            offset += 2;
            uint16_t result_addr = array[offset] | (array[offset + 1] << 8);
            offset += 2;
            return std::make_unique<MateAccept>(requester_addr, result_addr);
        }},
        { static_cast<uint8_t>(Id::ROCK_PAPER_SCISSORS), [](const std::vector<uint8_t>& array, size_t& offset) -> std::unique_ptr<Instruction> {
            if (array.size() - offset < 7) return nullptr;
            offset++;
            uint16_t choice_addr = array[offset] | (array[offset + 1] << 8);
            offset += 2;
            uint16_t opponent_addr = array[offset] | (array[offset + 1] << 8);
            offset += 2;
            uint16_t result_addr = array[offset] | (array[offset + 1] << 8);
            offset += 2;
            return std::make_unique<RockPaperScissors>(choice_addr, opponent_addr, result_addr);
        }}
    };

    if (array.size() - offset < 1) {
        return nullptr;
    }

    auto it = FACTORIES_TABLE.find(array[0]);
    if (it == FACTORIES_TABLE.end()) {
        return nullptr;
    }

    return it->second(array, offset);
}
