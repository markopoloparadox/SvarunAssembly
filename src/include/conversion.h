#pragma once
#include <optional>
#include <string_view>
#include "SvarunCommon/types.h"
#include "SvarunCommon/strongtypes.h"


std::optional<types::OpCode> StringToOpCode(std::string_view str);
std::optional<types::Register> StringToRegister(std::string_view str);
std::optional<Word> StringToNumber(std::string_view str);
std::optional<std::string> StringToLabelStart(std::string_view str);
std::optional<std::string> StringToLabel(std::string_view str);
std::optional<types::Memory> StringToMemory(std::string_view str);

template<typename T>
void InsertCode(SourceCode& code, T value) {
  auto ptr = (uint8_t*)&value;
  for (int i = 0; i < sizeof(T); i += 1) {
    code.push_back(*ptr);
    ptr += 1;
  }
}

