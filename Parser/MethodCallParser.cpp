
#include "MethodCallParser.h"

#include "ParserManager.h"
#include "Models/Ast/VariableAstNode.h"

std::shared_ptr<MethodCallAstNode> MethodCallParser::Parse() const
{
    const auto& methodCallAstNode = ParseExpression();

    if (!parser->TryConsumeOrError(TokenKind::semicolon))
        return nullptr;

    return methodCallAstNode;
}

std::shared_ptr<MethodCallAstNode> MethodCallParser::ParseExpression() const
{
    Token out_peekedToken = parser->PeekNextToken();
    std::shared_ptr<MemberAccessAstNode> lastMemberAccessAstNode;

    while (true)
    {
        const Token identifierToken = parser->GetNextToken();
        assert(identifierToken.kind == TokenKind::identifier);

        auto memberAccessAstNode = std::make_shared<MemberAccessAstNode>();
        memberAccessAstNode->member = identifierToken.text;
        memberAccessAstNode->token = identifierToken;

        if (lastMemberAccessAstNode != nullptr)
            memberAccessAstNode->object = lastMemberAccessAstNode;

        lastMemberAccessAstNode = memberAccessAstNode;

        out_peekedToken = parser->PeekNextToken();

        if (parser->TryConsumeOnMatchOrSkip(TokenKind::dot))
            continue;

        if (out_peekedToken.kind == TokenKind::lParen)
            break;
    }

    assert(lastMemberAccessAstNode != nullptr);

    auto methodCallAstNode = std::make_shared<MethodCallAstNode>();
    methodCallAstNode->callee = lastMemberAccessAstNode;
    methodCallAstNode->token = lastMemberAccessAstNode->token;

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

    return methodCallAstNode;
}
