#include <algorithm>
#include <iostream>
#include "token.h"
#include "SvarunCommon/types.h"
#include "opcodes.h"
#include "conversion.h"
#include "SvarunCommon/constants.h"
#include <fstream>
#include <map>
#include <utility>


std::map<std::string, Word> g_labels;
std::vector<std::pair<Word, std::string>> g_missingLabels;

SourceCode g_code;
std::vector<Token> g_tokens;
int g_token_position = 0;

std::string ReadFile();
bool Tokenize(std::string_view file);
bool ParseTokens();

void SaveToFile();
bool FillMissingTokens();

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

  if (!FillMissingTokens()) {
    std::cout << "Failed to add missing labels.";
    return 0;
  }

  SaveToFile();

  return 0;
}
bool FillMissingTokens() {
  for (auto& label : g_missingLabels) {
    if (g_labels.find(label.second) == g_labels.end()) {
      return false;
    }
    *(Word*)&g_code[label.first] = g_labels[label.second];
  }
  return true;
}


void SaveToFile() {
  std::ofstream myfile;
  myfile.open("output.s_asm", std::ios::binary);
  myfile.write((char*)g_code.data(), g_code.size());
  myfile.close();
}

std::string ReadFile() {

  std::string command = 
    "sub     sp, sp, 16 "
    "str     r0, sp, 12 "
    "str     r0, sp, 8 "
    ".L3: "
    "ldr     r0, sp, 8 "
    "cmp     r0, 4 "
    "jgt     .L2 "
    "ldr     r0, sp, 8 "
    "ldr     r1, sp, 12 "
    "add     r0, r1, r0 "
    "str     r0, sp, 12 "
    "ldr     r0, sp, 8 "
    "add     r0, r0, 1 "
    "str     r0, sp, 8 "
    "jmp      .L3 "
    ".L2: "
    "mov     r0, 1 "
    "str     r0, sp, 4 ";

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
    if (t->m_type == TokenType::LABEL_START) {
      g_labels[t->m_value] = g_code.size();
      continue;
    }

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
    case constants::NOP:
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
    case constants::JMP:
      success = ParseBranch(g_tokens, g_code, g_missingLabels, constants::JMP);
      break;
    case constants::JEQ:
      success = ParseBranch(g_tokens, g_code, g_missingLabels, constants::JEQ);
      break;
    case constants::JNE:
      success = ParseBranch(g_tokens, g_code, g_missingLabels, constants::JNE);
      break;
    case constants::JLT:
      success = ParseBranch(g_tokens, g_code, g_missingLabels, constants::JLT);
      break;
    case constants::JLE:
      success = ParseBranch(g_tokens, g_code, g_missingLabels, constants::JLE);
      break;
    case constants::JGT:
      success = ParseBranch(g_tokens, g_code, g_missingLabels, constants::JGT);
      break;
    case constants::JGE:
      success = ParseBranch(g_tokens, g_code, g_missingLabels, constants::JGE);
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
    case constants::LDRH:
      success = ParseCommon1(g_tokens, g_code, constants::LDRH);
      break;
    case constants::LDRB:
      success = ParseCommon1(g_tokens, g_code, constants::LDRB);
      break;
    case constants::STR:
      success = ParseCommon1(g_tokens, g_code, constants::STR);
      break;
    case constants::STRH:
      success = ParseCommon1(g_tokens, g_code, constants::STRH);
      break;
    case constants::STRB:
      success = ParseCommon1(g_tokens, g_code, constants::STRB);
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