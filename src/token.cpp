#include "token.h"
#include "conversion.h"

std::optional<Token> CreateToken(std::string_view token_text) {
  Token t;

  if (auto val = StringToOpCode(token_text)) {
    t.m_type = TokenType::OPCODE;
  }
  else if (auto val = StringToRegister(token_text)) {
    t.m_type = TokenType::REGISTER;
  }
  else if (auto val = StringToDigit(token_text)) {
    t.m_type = TokenType::DIGIT;
  }
  else {
    return {};
  }

  t.m_value = token_text;
  return t;
}

std::optional<Token> GetNextToken(std::vector<Token>& tokens) {
  static int i = 0;
  if (i >= tokens.size()) {
    return {};
  }

  return tokens[i++];
}