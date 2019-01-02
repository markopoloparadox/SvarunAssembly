#pragma once
#include "token.h"
#include "SvarunCommon/types.h"
#include "SvarunCommon/constants.h"

bool Parse1R0O(Tokens& tokens, SourceCode& code, constants::OpCode opcode);
bool Parse0R1O(Tokens& tokens, SourceCode& code, constants::OpCode opcode);
bool Parse1R1O(Tokens& tokens, SourceCode& code, constants::OpCode opcode);
bool Parse2R1O(Tokens& tokens, SourceCode& code, constants::OpCode opcode);
bool ParseBranch(Tokens& tokens, SourceCode& code, std::vector<std::pair<Word, std::string>>& missingLabels, constants::OpCode opcode);