#pragma once
#include "Models/CompilerContext.h"
#include "Models/Ast/MethodAstNode.h"

class MethodHasTransmitStatementChecker
{
public:
    static void ValidateMethod(CompilerContext* compilerContext, const MethodAstNode& methodAstNode);
};
