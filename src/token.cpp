#include "token.h"
#include "conversion.h"

std::optional<Token> CreateToken(std::string_view token_text) {
  Token t;

  if (auto val = StringToOpCode(token_text)) {
    t.m_type = TokenType::OPCODE;
    t.m_value = *val;
  }
  else if (auto val = StringToRegister(token_text)) {
    t.m_type = TokenType::REGISTER;
    t.m_value = *val;
  }
  else if (auto val = StringToNumber(token_text)) {
    t.m_type = TokenType::NUMBER;
    t.m_value = *val;
  }
  else if (auto val = StringToLabelStart(token_text)) {
    t.m_type = TokenType::LABEL_START;
    t.m_value = *val;
  }
  else if (auto val = StringToLabel(token_text)) {
    t.m_type = TokenType::LABEL;
    t.m_value = *val;
  }
  else if (auto val = StringToMemory(token_text)) {
    t.m_type = TokenType::MEMORY;
    t.m_value = *val;
  }
  else {
    return {};
  }

  return t;
}

std::optional<Token> GetNextToken(std::vector<Token>& tokens) {
  static int i = 0;
  if (i >= tokens.size()) {
    return {};
  }

  return tokens[i++];
}