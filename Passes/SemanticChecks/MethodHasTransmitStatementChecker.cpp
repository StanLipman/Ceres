#include "MethodHasTransmitStatementChecker.h"

#include "Models/Ast/TransmitAstNode.h"

void MethodHasTransmitStatementChecker::ValidateMethod(CompilerContext* compilerContext, const MethodAstNode& methodAstNode)
{
    if (methodAstNode.returnTypeAstNode == nullptr)
        return;

    bool processedTransmitStatement = false;

    for (const auto& statementAstNode : methodAstNode.bodyAstNode->statementAstNodes)
    {
        const auto* transmitStatement = dynamic_cast<TransmitAstNode*>(statementAstNode.get());
        if (transmitStatement == nullptr)
            continue;

        processedTransmitStatement = true;
        break;
    }

    if (!processedTransmitStatement)
    {
        std::string formattedMessage = "Method '" + methodAstNode.name + "' is missing a transmit statement.";
        compilerContext->compilationErrors.emplace_back(formattedMessage, methodAstNode.token.location);
    }
}
