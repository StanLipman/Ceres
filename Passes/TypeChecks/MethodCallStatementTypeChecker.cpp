#include "MethodCallStatementTypeChecker.h"

#include <iostream>
#include <format>

#include "Models/Symbols/MethodSymbol.h"
#include "Passes/TypeChecker.h"

void MethodCallStatementTypeChecker::ValidateStatement(CompilerContext* compilerContext, const MethodAstNode& methodAstNode)
{
    for (const auto& statementAstNode : methodAstNode.bodyAstNode->statementAstNodes)
    {
        const auto* methodCallStatement = dynamic_cast<MethodCallAstNode*>(statementAstNode.get());
        if (methodCallStatement == nullptr)
            continue;

        ValidateMethodCall(compilerContext, methodAstNode, *methodCallStatement);
    }
}

void MethodCallStatementTypeChecker::ValidateMethodCall(CompilerContext* compilerContext, const MethodAstNode& methodAstNode, const MethodCallAstNode& methodCallAstNode)
{
    const auto& calleeAstNode = methodCallAstNode.callee;
    assert(calleeAstNode != nullptr);

    const auto& symbol = GetMethodSymbol(compilerContext, methodAstNode, calleeAstNode);
    assert(symbol != nullptr);

    const auto& callingMethodSymbol = std::dynamic_pointer_cast<MethodSymbol>(symbol);

    if (callingMethodSymbol == nullptr)
    {
        const std::string formattedMessage = "Method '" + calleeAstNode->member + "' does not exist.";
        const SourceLocation location = methodCallAstNode.token.location;
        compilerContext->compilationErrors.emplace_back(formattedMessage, location);
        return;
    }

    if (callingMethodSymbol->type->parameterTypes.size() != methodCallAstNode.arguments.size())
    {
        const std::string formattedMessage = "Method '" + calleeAstNode->member +
            "' got " + std::to_string(methodCallAstNode.arguments.size()) + " arguments, but expected " +
            std::to_string(callingMethodSymbol->type->parameterTypes.size()) + " arguments.";

        const SourceLocation location = methodCallAstNode.token.location;
        compilerContext->compilationErrors.emplace_back(formattedMessage, location);
        return;
    }

    for (size_t i = 0; i < methodCallAstNode.arguments.size(); i++)
    {
        assert(i < callingMethodSymbol->type->parameterTypes.size());

        const auto& providedArgument = methodCallAstNode.arguments[i];
        assert(providedArgument != nullptr);
        const auto& providedArgumentType = TypeChecker::GetType(methodAstNode.scope, providedArgument);
        if (providedArgumentType == nullptr)
        {
            const auto& variableArgument = std::dynamic_pointer_cast<VariableAstNode>(providedArgument);
            assert(variableArgument != nullptr);
            const std::string formattedMessage = "'" + variableArgument->name+ "' could not get resolved.";
            compilerContext->compilationErrors.emplace_back(formattedMessage, variableArgument->token.location);
            return;
        }

        const auto& expectedArgumentType = callingMethodSymbol->type->parameterTypes[i];
        assert(expectedArgumentType != nullptr);

        if (providedArgumentType != expectedArgumentType)
        {
            assert(providedArgumentType->kind != Type::TypeKind::none);
            assert(expectedArgumentType->kind != Type::TypeKind::none);

            const std::string formattedMessage = "Method '" + calleeAstNode->member + "' expected argument with type of '" + expectedArgumentType->GetTypeKindText() + "' but got '" + providedArgumentType->GetTypeKindText() + "' argument.";
            compilerContext->compilationErrors.emplace_back(formattedMessage, methodCallAstNode.token.location);
        }
    }
}

std::shared_ptr<Symbol> MethodCallStatementTypeChecker::GetMethodSymbol(CompilerContext* compilerContext, const MethodAstNode& methodAstNode, const std::shared_ptr<MemberAccessAstNode>& memberAccessAstNode)
{
    auto symbol = methodAstNode.scope->Lookup(memberAccessAstNode->member);

    if (symbol != nullptr)
        return symbol;

    if (memberAccessAstNode->object == nullptr)
        return nullptr;

    symbol = GetMethodSymbol(compilerContext, methodAstNode, memberAccessAstNode->object);

    if (symbol == nullptr)
    {
        std::string formattedMsg = "Failed to find member '" + memberAccessAstNode->object->member + "'";
        compilerContext->compilationErrors.emplace_back(formattedMsg, memberAccessAstNode->object->token.location);
        return nullptr;
    }

    const auto& constellationSymbol = std::dynamic_pointer_cast<ConstellationSymbol>(symbol);
    assert(constellationSymbol != nullptr);
    assert(constellationSymbol->declaration != nullptr);
    assert(constellationSymbol->declaration->scope != nullptr);

    const auto& methodSymbol = constellationSymbol->declaration->scope->Lookup(memberAccessAstNode->member);
    if (methodSymbol == nullptr)
    {
        std::string formattedMsg = "Failed to find method '" + memberAccessAstNode->member + "'";
        compilerContext->compilationErrors.emplace_back(formattedMsg, memberAccessAstNode->token.location);
        return nullptr;
    }

    return methodSymbol;
}
