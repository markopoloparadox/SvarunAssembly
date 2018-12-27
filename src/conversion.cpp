#include "conversion.h"
#include <utility>
#include <map>
#include <string> 
#include "SvarunCommon/constants.h"

static const std::map<std::string, Byte> OpCodeLookup =
{
 {"noop", constants::NOOP},
 {"add", constants::ADD},
 {"sub", constants::SUB},
 {"and", constants::AND},
 {"eor", constants::EOR},
 {"orr", constants::ORR},
 {"mov", constants::MOV},
 {"cmp", constants::CMP},
 {"ifeq", constants::IFEQ},
 {"ifne", constants::IFNE},
 {"iflt", constants::IFLT},
 {"ifle", constants::IFLE},
 {"ifgt", constants::IFGT},
 {"ifge", constants::IFGE},
 {"push", constants::PUSH},
 {"pop", constants::POP},
 {"ldr", constants::LDR},
 {"str", constants::STR}
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
  return std::stoi(std::string(str));
}