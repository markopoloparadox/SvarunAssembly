#pragma once
#include <string>
#include <optional>
#include <string_view>
#include <variant>
#include "SvarunCommon/types.h"
#include "SvarunCommon/strongtypes.h"
#include <utility>

enum class TokenType {
  OPCODE = 1,
  REGISTER,
  NUMBER,
  LABEL_START,
  LABEL,
  MEMORY
};

struct Token {
  TokenType m_type;
  std::variant<types::OpCode, types::Register, types::Memory, Word, std::string> m_value;
};

using Tokens = std::vector<Token>;

std::optional<Token> CreateToken(std::string_view token_text);
std::optional<Token> GetNextToken(std::vector<Token>& tokens);
