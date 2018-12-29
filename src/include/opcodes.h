#pragma once
#include "token.h"
#include "SvarunCommon/types.h"
#include "SvarunCommon/constants.h"

bool ParseCommon1(Tokens& tokens, SourceCode& code, constants::OpCode opcode);
bool ParseCommon2(Tokens& tokens, SourceCode& code, constants::OpCode opcode);
bool ParseBranch(Tokens& tokens, SourceCode& code, constants::OpCode opcode);
bool ParsePush(Tokens& tokens, SourceCode& code);
bool ParsePop(Tokens& tokens, SourceCode& code);