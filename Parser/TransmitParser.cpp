#include "TransmitParser.h"

#include "ParserManager.h"
#include "Models/Ast/MethodAstNode.h"
#include "Models/Ast/VariableAstNode.h"

void TransmitParser::Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<TransmitAstNode>& transmitAstNode)
{
    const auto methodAstNode = std::dynamic_pointer_cast<MethodAstNode>(parent);
    assert(methodAstNode != nullptr); // What other parent can transmit values?

    assert(methodAstNode->bodyAstNode != nullptr);
    methodAstNode->bodyAstNode->statementAstNodes.push_back(transmitAstNode);

    const Token peekToken = parser->PeekNextToken();

    if (IsLiteralValueTokenKind(peekToken.kind))
    {
        assert(transmitAstNode->transmitValue == nullptr);

        if (const Token literalToken = parser->PeekNextToken(); literalToken.kind == TokenKind::_string || literalToken.kind == TokenKind::_char)
        {
            const auto& stringAstNode = std::make_shared<LiteralStringValueAstNode>(literalToken);
            parser->literalStringValueParser->Parse(transmitAstNode, stringAstNode);
        }
        else
        {
            const auto& numericAstNode = std::make_shared<LiteralNumericValueAstNode>(literalToken);
            parser->literalNumericValueParser->Parse(transmitAstNode, numericAstNode);
        }
    }
    else if (peekToken.kind == TokenKind::identifier)
    {
        const Token identifierToken = parser->GetNextToken();
        const auto& variableAstNode = std::make_shared<VariableAstNode>();
        variableAstNode->token = identifierToken;
        variableAstNode->name = identifierToken.text;

        assert(transmitAstNode->transmitValue == nullptr);
        transmitAstNode->transmitValue = variableAstNode;
    }

    if (const Token token = parser->GetNextToken(); token.kind != TokenKind::semicolon)
    {
        parser->GiveError(token, TokenKind::semicolon);
        return;
    }
}
