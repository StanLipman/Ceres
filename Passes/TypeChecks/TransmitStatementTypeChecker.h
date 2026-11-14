#pragma once
#include "Models/Ast/MethodAstNode.h"
#include "Models/Ast/TransmitAstNode.h"

class TypeChecker;

class TransmitStatementTypeChecker
{
public:
    static void ValidateStatement(CompilerContext* compilerContext, const MethodAstNode& methodAstNode, const TransmitAstNode& transmitAstNode);
};
