#pragma once
#include <string>
#include <optional>
#include <string_view>
#include <vector>

enum class TokenType {
  OPCODE = 1,
  REGISTER,
  DIGIT,
  LABEL_START,
  LABEL
};

struct Token {
  TokenType m_type;
  std::string m_value;
};

using Tokens = std::vector<Token>;

std::optional<Token> CreateToken(std::string_view token_text);
std::optional<Token> GetNextToken(std::vector<Token>& tokens);
