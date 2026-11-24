#include "ExpressionStatementTypeChecker.h"

#include "Models/Ast/ExpressionAstNode.h"
#include "Passes/AutoResolver.h"
#include "Passes/TypeChecker.h"

void ExpressionStatementTypeChecker::ValidateVariableDeclarationStatement(CompilerContext* compilerContext, const std::shared_ptr<Scope>& scope, const VariableDeclarationAstNode& variableDeclarationAstNode)
{
    assert(variableDeclarationAstNode.type != nullptr);
    const auto& typeTypeAstNode = std::dynamic_pointer_cast<TypeAstNode>(variableDeclarationAstNode.type);
    std::shared_ptr<Type> typeType;

    if (typeTypeAstNode != nullptr)
    {
        typeType = TypeChecker::GetType(scope, typeTypeAstNode.get());
    }
    else
    {
        assert(std::dynamic_pointer_cast<AutoAstNode>(variableDeclarationAstNode.type) != nullptr);
        typeType = AutoResolver::ResolveType(scope, variableDeclarationAstNode);
    }
    assert(typeType != nullptr); // Should have been handled in DeclarationResolver.h

    const auto& assignee = variableDeclarationAstNode.assignmentExpression->assignee;
    if (const auto& literalValueAstNode = std::dynamic_pointer_cast<LiteralValueAstNode>(assignee); literalValueAstNode != nullptr)
    {
        const auto& literalValueType = TypeChecker::GetType(scope, literalValueAstNode.get());
        assert(literalValueType != nullptr);

        if (literalValueType == typeType)
            return;

        std::string formattedMsg = "'" + variableDeclarationAstNode.name + "' expected type '" + typeTypeAstNode->name + "' but got '" + literalValueType->GetTypeKindText() + "'.";
        compilerContext->compilationErrors.emplace_back(formattedMsg, literalValueAstNode->token.location);
    }
    else if (const auto& variableValueAstNode = std::dynamic_pointer_cast<VariableAstNode>(assignee); variableValueAstNode != nullptr)
    {
        const auto& symbol = scope->Lookup(variableValueAstNode->name);
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
        assert(methodCallAstNode->callee != nullptr);
        const auto& symbol = MethodCallStatementTypeChecker::GetMethodSymbol(compilerContext, scope, methodCallAstNode->callee);

        const auto& callingMethodSymbol = std::dynamic_pointer_cast<MethodSymbol>(symbol);
        assert(callingMethodSymbol != nullptr);

        const auto& callingMethodDeclaration = callingMethodSymbol->declaration;
        assert(callingMethodDeclaration != nullptr);
        assert(callingMethodDeclaration->returnTypeAstNode != nullptr);
        const auto& callingMethodReturnType = TypeChecker::GetType(scope, callingMethodDeclaration->returnTypeAstNode.get());
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
