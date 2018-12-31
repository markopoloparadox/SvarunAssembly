#include "conversion.h"
#include <utility>
#include <map>
#include <string> 
#include "SvarunCommon/constants.h"

static const std::map<std::string, Byte> OpCodeLookup =
{
 {"nop", constants::NOP},
 {"add", constants::ADD},
 {"sub", constants::SUB},
 {"and", constants::AND},
 {"eor", constants::EOR},
 {"orr", constants::ORR},
 {"mov", constants::MOV},
 {"cmp", constants::CMP},
 {"jmp", constants::JMP},
 {"jeq", constants::JEQ},
 {"jne", constants::JNE},
 {"jlt", constants::JLT},
 {"jle", constants::JLE},
 {"jgt", constants::JGT},
 {"jge", constants::JGE},
 {"push", constants::PUSH},
 {"pop", constants::POP},
 {"ldr", constants::LDR},
 {"ldrh", constants::LDRH},
 {"ldrb", constants::LDRB},
 {"str", constants::STR},
 {"strh", constants::STRH},
 {"strb", constants::STRB}
};

static const std::map<std::string, Byte> RegisterLookup =
{
 {"r0", constants::R0},
 {"r1", constants::R1},
 {"r2", constants::R2},
 {"r3", constants::R3},
 {"r4", constants::R4},
 {"r5", constants::R5},
 {"r6", constants::R6},
 {"r7", constants::R7},
 {"r8", constants::R8},
 {"r9", constants::R9},
 {"sp", constants::SP}
};


std::optional<Byte> StringToOpCode(std::string_view str) {
  auto it = OpCodeLookup.find(std::string(str));
  if (it == OpCodeLookup.end()) {
    return {};
  }

  return (*it).second;
}


std::optional<Byte> StringToRegister(std::string_view str) {
  auto it = RegisterLookup.find(std::string(str));
  if (it == RegisterLookup.end()) {
    return {};
  }

  return (*it).second;
}


std::optional<Word> StringToDigit(std::string_view str) {
  try {
    return std::stoi(std::string(str));
  }
  catch (std::exception& e) {
    return {};
  }

  return {};
}


std::optional<bool> StringToLabelStart(std::string_view str) {
  if (str.front() == '.' && str.back() == ':') {
    return true;
  }

  return {};
}


std::optional<bool> StringToLabel(std::string_view str) {
  if (str.front() == '.') {
    return true;
  }

  return {};
}