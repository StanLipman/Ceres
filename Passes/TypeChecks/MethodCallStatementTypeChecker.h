#pragma once

#include "Models/CompilerContext.h"
#include "Models/Ast/MethodAstNode.h"
#include "Models/Ast/MethodCallAstNode.h"


class MethodCallStatementTypeChecker
{
public:
    static void ValidateStatement(CompilerContext* compilerContext, const MethodAstNode& methodAstNode, const MethodCallAstNode& methodCallAstNode);
    static std::shared_ptr<Symbol> GetMethodSymbol(CompilerContext* compilerContext, const MethodAstNode& methodAstNode, const std::shared_ptr<MemberAccessAstNode>& memberAccessAstNode);
};
