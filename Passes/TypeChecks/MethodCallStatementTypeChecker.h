#pragma once

#include "Models/CompilerContext.h"
#include "Models/Ast/MethodAstNode.h"
#include "Models/Ast/MethodCallAstNode.h"

class MethodCallStatementTypeChecker
{
public:
    static void ValidateMethodCall(CompilerContext* compilerContext, const std::shared_ptr<Scope>& scope, const MethodCallAstNode& methodCallAstNode);
    static std::shared_ptr<Symbol> GetMethodSymbol(CompilerContext* compilerContext, const std::shared_ptr<Scope>& scope, const std::shared_ptr<MemberAccessAstNode>& memberAccessAstNode);
};
