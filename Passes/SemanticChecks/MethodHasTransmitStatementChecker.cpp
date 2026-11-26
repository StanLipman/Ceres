#include "MethodHasTransmitStatementChecker.h"

#include "Models/Ast/TransmitAstNode.h"
#include "Passes/SemanticAnalyzer.h"
#include "Passes/TypeChecker.h"

void MethodHasTransmitStatementChecker::ValidateMethod(CompilerContext* compilerContext, const MethodAstNode& methodAstNode)
{
    const auto& returnTypeType = TypeChecker::GetType(methodAstNode.scope, methodAstNode.returnTypeAstNode.get());
    assert(returnTypeType != nullptr);

    const auto& symbol = methodAstNode.scope->Lookup("vacuum");
    const auto& vacuumSymbol = std::dynamic_pointer_cast<ConstellationSymbol>(symbol);
    assert(vacuumSymbol != nullptr);

    if (returnTypeType == vacuumSymbol->type)
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
