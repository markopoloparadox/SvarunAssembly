#pragma once
#include <optional>
#include <string_view>
#include "SvarunCommon/types.h"


std::optional<Byte> StringToOpCode(std::string_view str);
std::optional<Byte> StringToRegister(std::string_view str);
std::optional<Word> StringToDigit(std::string_view str);
std::optional<bool> StringToLabelStart(std::string_view str);
std::optional<bool> StringToLabel(std::string_view str);

template<typename T>
void InsertCode(SourceCode& code, T value) {
  auto ptr = (uint8_t*)&value;
  for (int i = 0; i < sizeof(T); i += 1) {
    code.push_back(*ptr);
    ptr += 1;
  }
}

