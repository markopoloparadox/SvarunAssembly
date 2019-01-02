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
  else if (auto val = StringToDigit(token_text)) {
    t.m_type = TokenType::DIGIT;
    t.m_value = *val;
  }
  else if (auto val = StringToLabelStart(token_text)) {
    t.m_type = TokenType::LABEL_START;
    token_text.remove_suffix(1);
    t.m_value = std::string(token_text);
  }
  else if (auto val = StringToLabel(token_text)) {
    t.m_type = TokenType::LABEL;
    t.m_value = std::string(token_text);
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