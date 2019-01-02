#include "opcodes.h"
#include "conversion.h"
#include "SvarunCommon/constants.h"

bool Parse0R1O(Tokens& tokens, SourceCode& code, constants::OpCode opcode) {
  auto token = GetNextToken(tokens);
  if (!token || (token->m_type != TokenType::DIGIT && token->m_type != TokenType::REGISTER)) {
    return false;
  }

  if (token->m_type == TokenType::DIGIT) {
    auto word = std::get<Word>(token->m_value);

    InsertCode(code, opcode);
    InsertCode(code, constants::OPERAND);
    InsertCode(code, word);
  }

  if (token->m_type == TokenType::REGISTER) {
    auto reg = std::get<Byte>(token->m_value);

    InsertCode(code, opcode);
    InsertCode(code, constants::REGISTER);
    InsertCode(code, reg);
  }

  return true;
}

bool Parse1R0O(Tokens& tokens, SourceCode& code, constants::OpCode opcode) {
  auto token = GetNextToken(tokens);
  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }

  auto reg = std::get<Byte>(token->m_value);

  InsertCode(code, opcode);
  InsertCode(code, reg);

  return true;
}

bool ParseBranch(Tokens& tokens, SourceCode& code, std::vector<std::pair<Word, std::string>>& missingLabels, constants::OpCode opcode) {
  auto token = GetNextToken(tokens);
  if (!token) {
    return false;
  }

  Word number = 0;
  if (token->m_type == TokenType::DIGIT) {
    number = std::get<Word>(token->m_value);
  }
  else if (token->m_type == TokenType::LABEL) {
    auto label = std::get<std::string>(token->m_value);
    missingLabels.push_back(std::make_pair(code.size() + 1, label));
  }
  else {
    return false;
  }

  InsertCode(code, opcode);
  InsertCode(code, number);
  return true;
}

bool Parse2R1O(Tokens& tokens, SourceCode& code, constants::OpCode opcode) {
  auto token = GetNextToken(tokens);

  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }
  auto regRt = std::get<Byte>(token->m_value);
  token = GetNextToken(tokens);

  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }
  auto regRn = std::get<Byte>(token->m_value);
  token = GetNextToken(tokens);

  if (!token || (token->m_type != TokenType::DIGIT && token->m_type != TokenType::REGISTER)) {
    return false;
  }

  if (token->m_type == TokenType::DIGIT) {
    Word word = std::get<Word>(token->m_value);

    InsertCode(code, opcode);
    InsertCode(code, constants::OPERAND);
    InsertCode(code, regRt);
    InsertCode(code, regRn);
    InsertCode(code, word);
  }

  if (token->m_type == TokenType::REGISTER) {
    auto regRm = std::get<Byte>(token->m_value);

    InsertCode(code, opcode);
    InsertCode(code, constants::REGISTER);
    InsertCode(code, regRt);
    InsertCode(code, regRn);
    InsertCode(code, regRm);
  }

  return true;
}

bool Parse1R1O(Tokens& tokens, SourceCode& code, constants::OpCode opcode) {
  auto token = GetNextToken(tokens);
  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }
  auto regRt = std::get<Byte>(token->m_value);
  token = GetNextToken(tokens);

  if (!token || (token->m_type != TokenType::DIGIT && token->m_type != TokenType::REGISTER)) {
    return false;
  }

  if (token->m_type == TokenType::DIGIT) {
    auto word = std::get<Word>(token->m_value);

    InsertCode(code, opcode);
    InsertCode(code, constants::OPERAND);
    InsertCode(code, regRt);
    InsertCode(code, word);
  }

  if (token->m_type == TokenType::REGISTER) {
    auto regRn = std::get<Byte>(token->m_value);

    InsertCode(code, opcode);
    InsertCode(code, constants::REGISTER);
    InsertCode(code, regRt);
    InsertCode(code, regRn);
  }

  return true;
}