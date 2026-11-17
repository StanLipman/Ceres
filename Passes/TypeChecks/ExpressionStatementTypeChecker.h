#pragma once
#include "Models/CompilerContext.h"
#include "Models/Ast/ExpressionAstNode.h"
#include "Models/Ast/MethodAstNode.h"

class ExpressionStatementTypeChecker
{
public:
    static void ValidateStatement(CompilerContext* compilerContext, const MethodAstNode& methodAstNode);
    static void ValidateVariableDeclarationStatement(CompilerContext* compilerContext, const MethodAstNode& methodAstNode, const VariableDeclarationAstNode& variableDeclarationAstNode);
};
