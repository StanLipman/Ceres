#include "ExpressionStatementTypeChecker.h"

#include "Models/Ast/ExpressionAstNode.h"
#include "Passes/TypeChecker.h"

void ExpressionStatementTypeChecker::ValidateStatement(CompilerContext* compilerContext, const MethodAstNode& methodAstNode)
{
    for (const auto& statementAstNode : methodAstNode.bodyAstNode->statementAstNodes)
    {
        const auto& variableDeclarationAstNode = std::dynamic_pointer_cast<VariableDeclarationAstNode>(statementAstNode);
        if (variableDeclarationAstNode == nullptr)
            continue;

        ValidateVariableDeclarationStatement(compilerContext, methodAstNode, *variableDeclarationAstNode);
    }
}

void ExpressionStatementTypeChecker::ValidateVariableDeclarationStatement(CompilerContext* compilerContext, const MethodAstNode& methodAstNode, const VariableDeclarationAstNode& variableDeclarationAstNode)
{
    assert(variableDeclarationAstNode.type != nullptr);
    const auto& typeTypeAstNode = variableDeclarationAstNode.type;
    const auto& typeType = TypeChecker::GetType(methodAstNode.scope, typeTypeAstNode);
    assert(typeType != nullptr); // Should have been handled in DeclarationResolver.h

    const auto& assignee = variableDeclarationAstNode.assignmentExpression->assignee;
    if (const auto& literalValueAstNode = std::dynamic_pointer_cast<LiteralValueAstNode>(assignee); literalValueAstNode != nullptr)
    {
        const auto& literalValueType = TypeChecker::GetType(methodAstNode.scope, literalValueAstNode);
        assert(literalValueType != nullptr);

        if (literalValueType == typeType)
            return;

        std::string formattedMsg = "'" + variableDeclarationAstNode.name + "' expected type '" + typeTypeAstNode->name + "' but got '" + literalValueType->GetTypeKindText() + "'.";
        compilerContext->compilationErrors.emplace_back(formattedMsg, literalValueAstNode->token.location);
    }
    else if (const auto& variableValueAstNode = std::dynamic_pointer_cast<VariableAstNode>(assignee); variableValueAstNode != nullptr)
    {
        const auto& symbol = methodAstNode.scope->Lookup(variableValueAstNode->name);
        if (const auto& variableSymbol = std::dynamic_pointer_cast<VariableSymbol>(symbol); variableSymbol != nullptr)
        {
            assert(variableSymbol->type != nullptr);
            const auto& variableType = variableSymbol->type;

            if (variableType == typeType)
                return;

            std::string formattedMsg = "'" + variableDeclarationAstNode.name + "' expected type '" + typeTypeAstNode->name + "' but got '" + variableType->GetTypeKindText() + "'.";
            compilerContext->compilationErrors.emplace_back(formattedMsg, variableValueAstNode->token.location);
        }
        else if (const auto& constellationSymbol = std::dynamic_pointer_cast<ConstellationSymbol>(symbol); constellationSymbol != nullptr)
        {
            assert(constellationSymbol->type != nullptr);
            const auto& constellationType = constellationSymbol->type;
            if (constellationType == typeType)
                return;

            std::string formattedMsg = "'" + variableDeclarationAstNode.name + "' expected type '" + typeTypeAstNode->name + "' but got '" + constellationSymbol->name + "'.";
            compilerContext->compilationErrors.emplace_back(formattedMsg, variableValueAstNode->token.location);
        }
        else
        {
            assert(false);
        }
    }
    else if (const auto& methodCallAstNode = std::dynamic_pointer_cast<MethodCallAstNode>(assignee); methodCallAstNode != nullptr)
    {
        MethodCallStatementTypeChecker::ValidateMethodCall(compilerContext, methodAstNode, *methodCallAstNode);

        assert(methodCallAstNode->callee != nullptr);
        const auto& symbol = MethodCallStatementTypeChecker::GetMethodSymbol(compilerContext, methodAstNode, methodCallAstNode->callee);

        const auto& callingMethodSymbol = std::dynamic_pointer_cast<MethodSymbol>(symbol);
        assert(callingMethodSymbol != nullptr);

        const auto& callingMethodDeclaration = callingMethodSymbol->declaration;
        assert(callingMethodDeclaration != nullptr);
        assert(callingMethodDeclaration->returnTypeAstNode != nullptr);
        const auto& callingMethodReturnType = TypeChecker::GetType(methodAstNode.scope, callingMethodDeclaration->returnTypeAstNode);
        assert(callingMethodReturnType != nullptr);

        if (callingMethodReturnType == typeType)
            return;

        std::string formattedMsg = "'" + variableDeclarationAstNode.name + "' expected type '" + typeTypeAstNode->name + "' but got '" + callingMethodReturnType->GetTypeKindText() + "'.";
        compilerContext->compilationErrors.emplace_back(formattedMsg, methodCallAstNode->token.location);
    }
    else
    {
        assert(false);
    }
}
