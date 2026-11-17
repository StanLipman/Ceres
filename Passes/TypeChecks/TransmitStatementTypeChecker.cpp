#include "TransmitStatementTypeChecker.h"

#include "Passes/TypeChecker.h"
#include "Models/Symbols/MethodSymbol.h"

void TransmitStatementTypeChecker::ValidateStatement(CompilerContext* compilerContext, const MethodAstNode& methodAstNode)
{
    bool processedTransmitStatement = false;

    for (const auto& statementAstNode : methodAstNode.bodyAstNode->statementAstNodes)
    {
        const auto* transmitStatement = dynamic_cast<TransmitAstNode*>(statementAstNode.get());
        if (transmitStatement == nullptr)
            continue;

        processedTransmitStatement = true;
        ValidateTransmitStatement(compilerContext, methodAstNode, *transmitStatement);
    }

    if (methodAstNode.returnTypeAstNode != nullptr && !processedTransmitStatement)
    {
        std::string formattedMessage = "Method '" + methodAstNode.name + "' is missing a transmit statement.";
        compilerContext->compilationErrors.emplace_back(formattedMessage, methodAstNode.token.location);
    }
}

void TransmitStatementTypeChecker::ValidateTransmitStatement(CompilerContext* compilerContext, const MethodAstNode& methodAstNode, const TransmitAstNode& transmitAstNode)
{
    const auto& transmitValue = transmitAstNode.transmitValue;

    assert(methodAstNode.scope != nullptr);
    const auto* methodSymbol = dynamic_cast<MethodSymbol*>(methodAstNode.scope->Lookup(methodAstNode.name).get());

    assert(methodSymbol != nullptr);
    assert(methodSymbol->type != nullptr);
    const auto& methodType = methodSymbol->type;
    assert(methodType->returnType != nullptr);

    const auto& returnType = methodType->returnType;
    assert(returnType != nullptr);

    const std::shared_ptr<Type>& transmitValueType = TypeChecker::GetType(methodAstNode.scope, transmitValue);
    if (transmitValueType != returnType)
    {
        std::string formattedMessage;
        SourceLocation location;

        if (transmitValue == nullptr)
        {
            formattedMessage = "Method '" + methodAstNode.name + "' expected transmit type of '" + methodAstNode.returnTypeAstNode->name + "', but got empty transmit.";
            location = transmitAstNode.token.location;
        }
        else if (const auto& literalValue = std::dynamic_pointer_cast<LiteralValueAstNode>(transmitValue); literalValue != nullptr)
        {
            const TokenKind transmitValueTokenKind = literalValue->tokenKind;
            formattedMessage = "Method '" + methodAstNode.name + "' expected transmit type of '" + methodAstNode.returnTypeAstNode->name + "', but '" + GetTokenKindText(transmitValueTokenKind) + "' got transmitted.";
            location = literalValue->token.location;
        }
        else if (const auto& variable = std::dynamic_pointer_cast<VariableAstNode>(transmitValue); variable != nullptr)
        {
            if (transmitValueType == nullptr)
            {
                formattedMessage = "Tried to transmit undefined value of name '" + variable->name + "'";
                location = transmitAstNode.token.location;
            }
            else
            {
                formattedMessage = "Method '" + methodAstNode.name + "' expected transmit type of '" + methodAstNode.returnTypeAstNode->name + "', but '" + transmitValueType->GetTypeKindText() + "' got transmitted.";
                location = variable->token.location;
            }
        }

        compilerContext->compilationErrors.emplace_back(formattedMessage, location);
    }
}
