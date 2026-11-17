#pragma once
#include "Models/CompilerContext.h"
#include "Models/Ast/MethodAstNode.h"

class ExpressionStatementTypeChecker
{
    static void ValidateStatement(CompilerContext* compilerContext, const MethodAstNode& methodAstNode);
};
