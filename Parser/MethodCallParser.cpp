
#include "MethodCallParser.h"

#include "ParserManager.h"
#include "Models/Ast/VariableAstNode.h"

std::shared_ptr<MethodCallAstNode> MethodCallParser::Parse() const
{
     // TODO: Check for constellation identifier: https://github.com/StanLipman/Ceres/issues/21
    const Token methodIdentifierToken = parser->GetNextToken();
    assert(methodIdentifierToken.kind == TokenKind::identifier);

    auto methodCallAstNode = std::make_shared<MethodCallAstNode>();
    methodCallAstNode->name = methodIdentifierToken.text;
    methodCallAstNode->token = methodIdentifierToken;

    if (!parser->TryConsumeOrError(TokenKind::lParen))
        return nullptr;

    Token rParenToken = parser->PeekNextToken();
    while (rParenToken.kind != TokenKind::rParen)
    {
        const Token peekedToken = parser->PeekNextToken();
        if (peekedToken.kind == TokenKind::identifier)
        {
            const Token identifierToken = parser->GetNextToken();
            const auto variableAstNode = std::make_shared<VariableAstNode>();
            variableAstNode->name = identifierToken.text;
            variableAstNode->token = identifierToken;

            methodCallAstNode->arguments.push_back(variableAstNode);
        }
        else if (IsLiteralValueTokenKind(peekedToken.kind))
        {
            const auto literalValueAstNode = parser->literalValueParser->Parse();

            methodCallAstNode->arguments.push_back(literalValueAstNode);
        }
        else if (parser->TryConsumeOnMatchOrSkip(TokenKind::comma))
        {
            continue;
        }
        else
        {
            const Token unknownToken = parser->GetNextToken();
            parser->GiveError(unknownToken, TokenKind::comma);
        }

        rParenToken = parser->PeekNextToken();
    }

    if (!parser->TryGetNextTokenOrError(TokenKind::rParen, rParenToken))
        return nullptr;

    if (!parser->TryConsumeOrError(TokenKind::semicolon))
        return nullptr;

    return methodCallAstNode;
}
