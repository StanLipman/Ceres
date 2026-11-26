#pragma once
#include "Models/Ast/MethodAstNode.h"
#include "Models/Ast/TransmitAstNode.h"

class TypeChecker;

class TransmitStatementTypeChecker
{
public:
    static void ValidateTransmitStatement(CompilerContext* compilerContext, const std::shared_ptr<Scope>& scope, const TransmitAstNode& transmitAstNode);
};
