#pragma once
#include "token.h"
#include "SvarunCommon/types.h"
#include "SvarunCommon/strongtypes.h"
#include "SvarunCommon/constants.h"

bool Parse1R1O(Tokens& tokens, SourceCode& code, types::OpCode opcode);
bool Parse2R1O(Tokens& tokens, SourceCode& code, types::OpCode opcode);
bool Parse2R2O(Tokens& tokens, SourceCode& code, types::OpCode opcode);
bool ParseBranch(Tokens& tokens, SourceCode& code, std::vector<std::pair<Word, std::string>>& missingLabels, types::OpCode opcode);