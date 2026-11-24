#pragma once
#include "Models/CompilerContext.h"
#include "Models/Ast/ExpressionAstNode.h"
#include "Models/Ast/MethodAstNode.h"

class ExpressionStatementTypeChecker
{
public:
    static void ValidateVariableDeclarationStatement(CompilerContext* compilerContext, const std::shared_ptr<Scope>& scope, const VariableDeclarationAstNode& variableDeclarationAstNode);
};
