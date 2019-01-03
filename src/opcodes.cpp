#include "opcodes.h"
#include "conversion.h"
#include "SvarunCommon/constants.h"

bool ParseBranch(Tokens& tokens, SourceCode& code, std::vector<std::pair<Word, std::string>>& missingLabels, types::OpCode opcode) {
  auto token = GetNextToken(tokens);
  if (!token) {
    return false;
  }

  Word number = 0;
  if (token->m_type == TokenType::NUMBER) {
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

bool Parse2R1O(Tokens& tokens, SourceCode& code, types::OpCode opcode) {
  auto token = GetNextToken(tokens);

  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }
  auto regRt = std::get<types::Register>(token->m_value);
  token = GetNextToken(tokens);

  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }
  auto regRn = std::get<types::Register>(token->m_value);
  token = GetNextToken(tokens);

  if (!token) {
    return false;
  }

  InsertCode(code, opcode.get());
  InsertCode(code, regRt.get());
  InsertCode(code, regRn.get());

  if (token->m_type == TokenType::NUMBER) {
    Word word = std::get<Word>(token->m_value);

    InsertCode(code, constants::NUMBER);
    InsertCode(code, word);
  }
  else if (token->m_type == TokenType::REGISTER) {
    auto regRm = std::get<types::Register>(token->m_value);

    InsertCode(code, constants::REGISTER);
    InsertCode(code, regRm.get());
  }
  else if (token->m_type == TokenType::MEMORY) {
    auto mem = std::get<types::Memory>(token->m_value);

    InsertCode(code, constants::MEMORY);
    InsertCode(code, mem.reg);
    InsertCode(code, mem.offset);
    InsertCode(code, mem.flag);
  }
  else {
    return false;
  }

  return true;
}

bool Parse1R1O(Tokens& tokens, SourceCode& code, types::OpCode opcode) {
  auto token = GetNextToken(tokens);
  if (!token || token->m_type != TokenType::REGISTER) {
    return false;
  }
  auto regRt = std::get<types::Register>(token->m_value);
  token = GetNextToken(tokens);

  if (!token) {
    return false;
  }

  InsertCode(code, opcode.get());
  InsertCode(code, regRt.get());

  if (token->m_type == TokenType::NUMBER) {
    Word word = std::get<Word>(token->m_value);

    InsertCode(code, constants::NUMBER);
    InsertCode(code, word);
  }
  else if (token->m_type == TokenType::REGISTER) {
    auto regRm = std::get<types::Register>(token->m_value);

    InsertCode(code, constants::REGISTER);
    InsertCode(code, regRm.get());
  }
  else if (token->m_type == TokenType::MEMORY) {
    auto mem = std::get<types::Memory>(token->m_value);

    InsertCode(code, constants::MEMORY);
    InsertCode(code, mem.reg);
    InsertCode(code, mem.offset);
    InsertCode(code, mem.flag);
  }
  else {
    return false;
  }

  return true;
}