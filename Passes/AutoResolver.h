#pragma once
#include <memory>

#include "TypeChecker.h"
#include "Models/Ast/AutoAstNode.h"
#include "Models/Ast/ExpressionAstNode.h"
#include "Models/Ast/LiteralValueAstNode.h"
#include "Models/Types/Type.h"

// As this name can be confusing: this is supposed to resolve the types for all the variables that use auto
class AutoResolver
{
public:
    static std::shared_ptr<Type> ResolveType(const std::shared_ptr<Scope>& scope, const VariableDeclarationAstNode& variableDeclarationAstNode)
    {
        assert(std::dynamic_pointer_cast<AutoAstNode>(variableDeclarationAstNode.type) != nullptr);
        assert(variableDeclarationAstNode.assignmentExpression != nullptr);
        assert(variableDeclarationAstNode.assignmentExpression->assignee != nullptr);

        const auto& typeForAuto = ResolveTypeFromExpression(nullptr, scope, *variableDeclarationAstNode.assignmentExpression->assignee);
        assert(typeForAuto != nullptr);
        return typeForAuto;
    }

    static std::shared_ptr<Type> ResolveTypeFromExpression(CompilerContext* compilerContext, const std::shared_ptr<Scope>& scope, const AstNode& astNode)
    {
        if (const auto* literalValueAstNode = dynamic_cast<const LiteralValueAstNode*>(&astNode); literalValueAstNode != nullptr)
        {
            const auto& literalValueType = TypeChecker::GetType(scope, literalValueAstNode);
            assert(literalValueType != nullptr);
            return literalValueType;
        }

        if (const auto* variableValueAstNode = dynamic_cast<const VariableAstNode*>(&astNode); variableValueAstNode != nullptr)
        {
            const auto& symbol = scope->Lookup(variableValueAstNode->name);

            if (const auto& variableSymbol = std::dynamic_pointer_cast<VariableSymbol>(symbol); variableSymbol != nullptr)
            {
                assert(variableSymbol->type != nullptr);
                return variableSymbol->type;
            }

            if (const auto& constellationSymbol = std::dynamic_pointer_cast<ConstellationSymbol>(symbol); constellationSymbol != nullptr)
            {
                assert(constellationSymbol->type != nullptr);
                return constellationSymbol->type;
            }

            assert(false); // Should never happen
            return nullptr;
        }

        if (const auto* methodCallAstNode = dynamic_cast<const MethodCallAstNode*>(&astNode); methodCallAstNode != nullptr)
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
            return callingMethodReturnType;
        }

        assert(false); // Should never happen
        return nullptr;
    }
};