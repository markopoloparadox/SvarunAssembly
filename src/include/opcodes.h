#pragma once
#include "token.h"
#include "SvarunCommon/types.h"

bool ParseSub(std::vector<Token>& tokens, SourceCode& code);
bool ParseMov(std::vector<Token>& tokens, SourceCode& code);
bool ParseAdd(std::vector<Token>& tokens, SourceCode& code);