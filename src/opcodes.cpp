#include "opcodes.h"
#include "conversion.h"
#include "SvarunCommon/constants.h"

bool ParseSub(std::vector<Token>& tokens, SourceCode& code) {
  auto token = GetNextToken(tokens);

  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }

  auto rt = StringToRegister(token->m_value);
  if (!rt) {
    return false;
  }

  token = GetNextToken(tokens);
  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }

  auto rn = StringToRegister(token->m_value);
  if (!rn) {
    return false;
  }

  token = GetNextToken(tokens);

  if (!token || (token->m_type != TokenType::DIGIT && token->m_type != TokenType::REGISTER)) {
    return false;
  }

  if (token->m_type == TokenType::DIGIT) {
    auto value = StringToDigit(token->m_value);
    if (!value) {
      return false;
    }

    InsertCode(code, constants::SUB);
    InsertCode(code, constants::OPERAND);
    InsertCode(code, *rt);
    InsertCode(code, *rn);
    InsertCode(code, *value);
  }

  if (token->m_type == TokenType::REGISTER) {
    auto reg = StringToRegister(token->m_value);
    if (!reg) {
      return false;
    }

    InsertCode(code, constants::SUB);
    InsertCode(code, constants::REGISTER);
    InsertCode(code, *rt);
    InsertCode(code, *rn);
    InsertCode(code, *reg);
  }

  return true;
}

bool ParseMov(std::vector<Token>& tokens, SourceCode& code) {
  auto token = GetNextToken(tokens);
  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }

  auto rt = StringToRegister(token->m_value);
  if (!rt) {
    return false;
  }

  token = GetNextToken(tokens);

  if (!token || (token->m_type != TokenType::DIGIT && token->m_type != TokenType::REGISTER)) {
    return false;
  }

  if (token->m_type == TokenType::DIGIT) {
    auto value = StringToDigit(token->m_value);
    if (!value) {
      return false;
    }

    InsertCode(code, constants::MOV);
    InsertCode(code, constants::OPERAND);
    InsertCode(code, *rt);
    InsertCode(code, *value);
  }

  if (token->m_type == TokenType::REGISTER) {
    auto reg = StringToRegister(token->m_value);
    if (!reg) {
      return false;
    }

    InsertCode(code, constants::MOV);
    InsertCode(code, constants::REGISTER);
    InsertCode(code, *rt);
    InsertCode(code, *reg);
  }

  return true;
}

bool ParseAdd(std::vector<Token>& tokens, SourceCode& code) {
  auto token = GetNextToken(tokens);

  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }

  auto rt = StringToRegister(token->m_value);
  if (!rt) {
    return false;
  }

  token = GetNextToken(tokens);
  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }


  auto rn = StringToRegister(token->m_value);
  if (!rn) {
    return false;
  }

  token = GetNextToken(tokens);

  if (!token || (token->m_type != TokenType::DIGIT && token->m_type != TokenType::REGISTER)) {
    return false;
  }

  if (token->m_type == TokenType::DIGIT) {
    auto value = StringToDigit(token->m_value);
    if (!value) {
      return false;
    }

    InsertCode(code, constants::ADD);
    InsertCode(code, constants::OPERAND);
    InsertCode(code, *rt);
    InsertCode(code, *rn);
    InsertCode(code, *value);
  }

  if (token->m_type == TokenType::REGISTER) {
    auto reg = StringToRegister(token->m_value);
    if (!reg) {
      return false;
    }

    InsertCode(code, constants::ADD);
    InsertCode(code, constants::REGISTER);
    InsertCode(code, *rt);
    InsertCode(code, *rn);
    InsertCode(code, *reg);
  }

  return true;
}