#include "opcodes.h"
#include "conversion.h"
#include "SvarunCommon/constants.h"

bool ParsePush(Tokens& tokens, SourceCode& code) {
  auto token = GetNextToken(tokens);
  if (!token || (token->m_type != TokenType::DIGIT && token->m_type != TokenType::REGISTER)) {
    return false;
  }

  if (token->m_type == TokenType::DIGIT) {
    auto value = StringToDigit(token->m_value);
    if (!value) {
      return false;
    }

    InsertCode(code, constants::PUSH);
    InsertCode(code, constants::OPERAND);
    InsertCode(code, *value);
  }

  if (token->m_type == TokenType::REGISTER) {
    auto reg = StringToRegister(token->m_value);
    if (!reg) {
      return false;
    }

    InsertCode(code, constants::PUSH);
    InsertCode(code, constants::REGISTER);
    InsertCode(code, *reg);
  }

  return true;
}

bool ParsePop(Tokens& tokens, SourceCode& code) {
  auto token = GetNextToken(tokens);
  if (!token || token->m_type != TokenType::DIGIT) {
    return false;
  }

  auto reg = StringToRegister(token->m_value);
  if (!reg) {
    return false;
  }

  InsertCode(code, constants::POP);
  InsertCode(code, *reg);

  return true;
}

bool ParseBranch(Tokens& tokens, SourceCode& code, std::vector<std::pair<Word, std::string>>& missingLabels, constants::OpCode opcode) {
  auto token = GetNextToken(tokens);
  if (!token) {
    return false;
  }

  Word number = 0;
  if (token->m_type == TokenType::DIGIT) {
    auto value = StringToDigit(token->m_value);
    if (!value) {
      return false;
    }
    number = *value;
  }
  else if (token->m_type == TokenType::LABEL) {
    missingLabels.push_back(std::make_pair(code.size() + 1, token->m_value));
    number = 0;
  }
  else {
    return false;
  }

  InsertCode(code, opcode);
  InsertCode(code, number);
  return true;
}

bool ParseCommon1(Tokens& tokens, SourceCode& code, constants::OpCode opcode) {
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

    InsertCode(code, opcode);
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

    InsertCode(code, opcode);
    InsertCode(code, constants::REGISTER);
    InsertCode(code, *rt);
    InsertCode(code, *rn);
    InsertCode(code, *reg);
  }

  return true;
}

bool ParseCommon2(Tokens& tokens, SourceCode& code, constants::OpCode opcode) {
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

    InsertCode(code, opcode);
    InsertCode(code, constants::OPERAND);
    InsertCode(code, *rt);
    InsertCode(code, *value);
  }

  if (token->m_type == TokenType::REGISTER) {
    auto reg = StringToRegister(token->m_value);
    if (!reg) {
      return false;
    }

    InsertCode(code, opcode);
    InsertCode(code, constants::REGISTER);
    InsertCode(code, *rt);
    InsertCode(code, *reg);
  }

  return true;
}