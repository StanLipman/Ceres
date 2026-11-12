#include "TransmitParser.h"
#include "ParserManager.h"
#include "Models/Ast/VariableAstNode.h"

std::shared_ptr<TransmitAstNode> TransmitParser::Parse() const
{
    const Token transmitToken = parser->GetNextToken();
    assert(transmitToken.kind == TokenKind::transmit);

    auto transmitAstNode = std::make_shared<TransmitAstNode>();
    transmitAstNode->token = transmitToken;

    const Token peekValueToken = parser->PeekNextToken();

    if (IsLiteralValueTokenKind(peekValueToken.kind))
    {
        const auto literalValueAstNode = parser->literalValueParser->Parse();
        if (literalValueAstNode == nullptr)
            return nullptr;

        transmitAstNode->transmitValue = literalValueAstNode;
    }
    else if (peekValueToken.kind == TokenKind::identifier)
    {
        const Token identifierToken = parser->GetNextToken();
        const auto variableAstNode = std::make_shared<VariableAstNode>();
        variableAstNode->token = identifierToken;
        variableAstNode->name = identifierToken.text;

        transmitAstNode->transmitValue = variableAstNode;
    }

    if (!parser->TryConsumeOrError(TokenKind::semicolon))
        return nullptr;

    return transmitAstNode;
}
