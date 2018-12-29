
/*
std::vector<uint8_t> g_code;
std::vector<Token> g_tokens;
int g_token_position = 0;

std::optional<Token> GetNextToken() {
  if (g_token_position == g_tokens.size()) {
    return {};
  }

  return g_tokens[g_token_position++];
}

std::string ReadFile();
void Tokenize(std::string_view line);
void Parse();

int main(int argc, char *argv[]) {
  std::cout << "Welcome to Ivana asembler\n";

  auto program = ReadFile();

  std::istringstream stream(program.c_str());

  std::string line;
  while (std::getline(stream, line)) {
    Tokenize(line);
  }
  Parse();


  return 0;
}

void Tokenize(std::string_view line) {
  while (1) {
    if (line.empty()) {
      return;
    }

    auto pos = line.find_first_of(' ');
    if (pos == std::string::npos) {
      auto token = CreateToken(line);
      if (token) {
        g_tokens.push_back(*token);
      }
      return;
    }

    auto token = CreateToken(line.substr(0, pos));
    if (token) {
      g_tokens.push_back(*token);
    }

    line = line.substr(pos + 1);
  }
}

bool ParseMov();
bool Parse2(Token& token) {
  std::string inst = token.m_value;

  if (inst == "mov") {
    return ParseMov();
  }

  return false;
}

void Parse() {
  while (auto t = GetNextToken()) {
    if (t->m_type != TokenType::INSTRUCTION) {
      std::cout << "Token is not a instruction!";
      return;
    }

    if (!Parse2(*t)) {
      std::cout << "Failed to parse instruction!";
      return;
    }

  }
}


bool ParseMov() {
  auto rt = GetNextToken();

  if (!rt || rt->m_type != TokenType::REGISTER) {
    std::cout << "rt is not a register!";
    return false;
  }

  auto rn = GetNextToken();

  if (!rn) {
    std::cout << "Token doesn't exists!1";
    return false;
  }

  if (rn->m_type == TokenType::REGISTER && rn->m_type == TokenType::DIGIT) {
    std::cout << "Token doesn't exists!2";
    return false;
  }

  InsertCode(g_code, (uint8_t)OpCode::MOV);

  if (rn->m_type == TokenType::REGISTER) {

    InsertCode(g_code, (uint8_t)ImmediateOperand::REGISTER);
    InsertCode(g_code, StringToRegister(rn->m_value));
    return true;
  }


  if (rn->m_type == TokenType::DIGIT) {
    InsertCode(g_code, (uint8_t)ImmediateOperand::OPERAND);
    InsertCode(g_code, (int32_t)std::stoi(rn->m_value));
    return true;
  }

  return false;
}




void ParseLine(std::string_view line) {
  std::vector<Token> tokens;
  std::string_view line_temp = line;
  while (1) {
    if (line_temp.empty()) {
      break;
    }

    auto empty_space = line_temp.find_first_of(' ');
    if (empty_space == std::string::npos) {
      auto token = CreateToken(line_temp);
      if (token) {
        tokens.push_back(*token);
      }
      break;
    }
    else{
      std::string_view tmp = line_temp.substr(0, empty_space);
      line_temp = line_temp.substr(empty_space + 1);
      auto token = CreateToken(tmp);
      if (token) {
        tokens.push_back(*token);
      }
    }
  }
}



std::string ReadFile() {
  std::string command = 
    //"sub sp, sp, 8\n"
    "mov r0, 1\n"
    "str r0, sp, 4\n"
    "mov r1, 1\n"
    "str r1, sp, 0\n";

  return command;
}
*/


#include <algorithm>
#include <iostream>
#include "token.h"
#include "SvarunCommon/types.h"
#include "opcodes.h"
#include "conversion.h"
#include "SvarunCommon/constants.h"
#include <fstream>


SourceCode g_code;
std::vector<Token> g_tokens;
int g_token_position = 0;

std::string ReadFile();
bool Tokenize(std::string_view file);
bool ParseTokens();

void SaveToFile();

int main(int argc, char *argv[]) {
  std::string file = ReadFile();
  std::replace(std::begin(file), std::end(file), '\n', ' ');
  if (!Tokenize(file)) {
    std::cout << "Failed to Tokenize.";
    return 0;
  }

  if (!ParseTokens()) {
    std::cout << "Failed to parse tokens.";
    return 0;
  }

  SaveToFile();

  return 0;
}

void SaveToFile() {
  std::ofstream myfile;
  myfile.open("output.s_asm", std::ios::binary);
  myfile.write((char*)g_code.data(), g_code.size());
  myfile.close();
}

std::string ReadFile() {
  std::string command =
    "sub     sp, sp, 8 "
    "mov     r0, 1 "
    "str     r0, sp, 4 "
    "mov     r1, 2 "
    "str     r1, sp, 0 "
    "mov     r2, 10 "
    "push r2 "
    "push -33 ";
  return command;
}


std::vector<std::string_view> GetWords(std::string_view text) {
  std::vector<std::string_view> words;

  while (!text.empty()) {
    while (!text.empty() && (text.front() == ' ' || text.front() == ',')) {
      text.remove_prefix(1);
    }

    if (text.empty()) {
      return words;
    }

    auto pos = text.find_first_of(' ');
    auto word = text.substr(0, pos);
    if (!word.empty() && word.back() == ',') {
      word.remove_suffix(1);
    }

    words.push_back(word);
    text.remove_prefix(pos + 1);
  }

  return words;
}


bool Tokenize(std::string_view file) {
  auto words = GetWords(file);
  for (auto& word : words) {
    auto token = CreateToken(word);
    if (token) {
      g_tokens.push_back(*token);
    }
    else {
      return false;
    }
  }
  return true;
}

bool ParseTokens() {
  while (auto t = GetNextToken(g_tokens)) {
    if (t->m_type != TokenType::OPCODE) {
      return false;
    }

    auto opcode = StringToOpCode(t->m_value);
    if (!opcode) {
      return false;
    }

    bool success = false;
    switch (*opcode)
    {
    case constants::NOOP:
      break;
    case constants::ADD:
      success = ParseCommon1(g_tokens, g_code, constants::ADD);
      break;
    case constants::SUB:
      success = ParseCommon1(g_tokens, g_code, constants::SUB);
      break;
    case constants::AND:
      success = ParseCommon1(g_tokens, g_code, constants::AND);
      break;
    case constants::EOR:
      success = ParseCommon1(g_tokens, g_code, constants::EOR);
      break;
    case constants::ORR:
      success = ParseCommon1(g_tokens, g_code, constants::ORR);
      break;
    case constants::MOV:
      success = ParseCommon2(g_tokens, g_code, constants::MOV);
      break;
    case constants::CMP:
      success = ParseCommon2(g_tokens, g_code, constants::CMP);
      break;
    case constants::IFEQ:
      success = ParseBranch(g_tokens, g_code, constants::IFEQ);
      break;
    case constants::IFNE:
      success = ParseBranch(g_tokens, g_code, constants::IFNE);
      break;
    case constants::IFLT:
      success = ParseBranch(g_tokens, g_code, constants::IFLT);
      break;
    case constants::IFLE:
      success = ParseBranch(g_tokens, g_code, constants::IFLE);
      break;
    case constants::IFGT:
      success = ParseBranch(g_tokens, g_code, constants::IFGT);
      break;
    case constants::IFGE:
      success = ParseBranch(g_tokens, g_code, constants::IFGE);
      break;
    case constants::PUSH:
      success = ParsePush(g_tokens, g_code);
      break;
    case constants::POP:
      success = ParsePop(g_tokens, g_code);
      break;
    case constants::LDR:
      success = ParseCommon1(g_tokens, g_code, constants::LDR);
      break;
    case constants::STR:
      success = ParseCommon1(g_tokens, g_code, constants::STR);
      break;
    default:
      return false;
      break;
    }
    if (!success) {
      return false;
    }

  }

  return true;
}