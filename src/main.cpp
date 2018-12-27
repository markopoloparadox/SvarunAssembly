
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
    "mov r0, 5\n"
    "sub r1, r0, 1\n"
    "add r2, r1, 2";

  return command;
}

bool Tokenize(std::string_view file) {
  while (1) {
    if (file.empty()) {
      break;
    }

    auto pos = file.find_first_of(' ');
    if (pos == std::string::npos) {
      auto token = CreateToken(file);
      if (token) {
        g_tokens.push_back(*token);
      }
      else {
        return false;
      }

      break;
    }

    auto token = CreateToken(file.substr(0, pos));
    if (token) {
      g_tokens.push_back(*token);
    }
    else {
      return false;
    }

    file = file.substr(pos + 1);
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
    switch (*opcode) {
    case constants::SUB:
      success = ParseSub(g_tokens, g_code);
      break;
    case constants::MOV:
      success = ParseMov(g_tokens, g_code);
      break;
    case constants::ADD:
      success = ParseAdd(g_tokens, g_code);
      break;
    default:
      return false;
    }

    if (!success) {
      return false;
    }

  }

  return true;
}