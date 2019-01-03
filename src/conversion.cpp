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
 {"b", constants::JMP},
 {"beq", constants::JEQ},
 {"bne", constants::JNE},
 {"blt", constants::JLT},
 {"ble", constants::JLE},
 {"bgt", constants::JGT},
 {"bge", constants::JGE},
 {"ldr", constants::LDR},
 {"ldrh", constants::LDRH},
 {"ldrb", constants::LDRB},
 {"str", constants::STR},
 {"strh", constants::STRH},
 {"strb", constants::STRB},
 {"mul", constants::MUL},
 {"lsl", constants::LSL},
 {"lsr", constants::LSR},
};

static const std::map<std::string, Byte> RegisterLookup =
{
 {"r0", constants::R0},
 {"w0", constants::R0},
 {"r1", constants::R1},
 {"w1", constants::R1},
 {"r2", constants::R2},
 {"w2", constants::R2},
 {"r3", constants::R3},
 {"w3", constants::R3},
 {"r4", constants::R4},
 {"w4", constants::R4},
 {"r5", constants::R5},
 {"w5", constants::R5},
 {"r6", constants::R6},
 {"w6", constants::R6},
 {"r7", constants::R7},
 {"w7", constants::R7},
 {"r8", constants::R8},
 {"w8", constants::R8},
 {"r9", constants::R9},
 {"w9", constants::R9},
 {"sp", constants::SP},
 {"zr", constants::ZR},
 {"wzr", constants::ZR}
};


std::optional<types::OpCode> StringToOpCode(std::string_view str) {
  auto it = OpCodeLookup.find(std::string(str));
  if (it == OpCodeLookup.end()) {
    return {};
  }

  return types::OpCode((*it).second);
}


std::optional<types::Register> StringToRegister(std::string_view str) {
  auto it = RegisterLookup.find(std::string(str));
  if (it == RegisterLookup.end()) {
    return {};
  }

  return types::Register((*it).second);
}


std::optional<Word> StringToNumber(std::string_view str) {
  try {
    return std::stoi(std::string(str));
  }
  catch (std::exception& e) {
    return {};
  }

  return {};
}


std::optional<std::string> StringToLabelStart(std::string_view str) {
  if (str.front() == '.' && str.back() == ':') {
    str.remove_suffix(1);
    return std::string(str);
  }

  return {};
}


std::optional<std::string> StringToLabel(std::string_view str) {
  if (str.front() == '.') {
    return std::string(str);
  }

  return {};
}

std::optional<types::Memory> StringToMemory(std::string_view str) {
  types::Memory mem;

  if (str.back() == '!') {
    mem.flag = 1;
    str.remove_suffix(1);
  }

  if (str.front() != '[' || str.back() != ']') {
    return {};
  }
  str.remove_prefix(1);
  str.remove_suffix(1);
  if (str.empty()) {
    return {};
  }

  auto delimiter = str.find(',');

  std::optional<types::Register> reg;
  if (delimiter == std::string::npos) {
    reg = StringToRegister(str);
  }
  else {
    reg = StringToRegister(str.substr(0, delimiter));
  }

  if (!reg) {
    return {};
  }

  mem.reg = *reg;
  if (delimiter == std::string::npos) {
    return mem;
  }
  
  str.remove_prefix(delimiter + 1);

  auto word = StringToNumber(str);
  if (!word) {
    return {};
  }

  mem.offset = *word;

  return mem;
}