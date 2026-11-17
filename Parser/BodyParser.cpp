#include "BodyParser.h"

#include "ParserManager.h"
#include "Models/Token.h"
#include "Models/Ast/ConstellationAstNode.h"
#include "Models/Ast/LiteralValueAstNode.h"
#include "Models/Ast/MethodAstNode.h"
#include "Models/Ast/MethodCallAstNode.h"
#include "Models/Ast/TransmitAstNode.h"

class GalaxyAstNode;

std::shared_ptr<BodyAstNode> BodyParser::ParseGalaxyBody() const
{
    Token lCurlyToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::lCurly, lCurlyToken))
        return nullptr;

    auto bodyAstNode = std::make_shared<BodyAstNode>();
    bodyAstNode->leftCurlyToken = lCurlyToken;

    Token peekedToken = parser->PeekNextToken();

    while (peekedToken.kind != TokenKind::rCurly)
    {
        const auto constellation = parser->constellationParser->Parse();
        if (constellation != nullptr)
            bodyAstNode->statementAstNodes.push_back(constellation);

        peekedToken = parser->PeekNextToken();
    }

    Token rCurlyToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::rCurly, rCurlyToken))
        return nullptr;

    bodyAstNode->rightCurlyToken = rCurlyToken;

    return bodyAstNode;
}

std::shared_ptr<BodyAstNode> BodyParser::ParseConstellationBody() const
{
    Token lCurlyToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::lCurly, lCurlyToken))
        return nullptr;

    auto bodyAstNode = std::make_shared<BodyAstNode>();
    bodyAstNode->leftCurlyToken = lCurlyToken;

    Token peekedToken = parser->PeekNextToken();

    while (peekedToken.kind != TokenKind::rCurly)
    {
        // TODO: Make handling between methods, and fields
        const auto methodAstNode = parser->methodParser->Parse();
        if (methodAstNode != nullptr)
            bodyAstNode->statementAstNodes.push_back(methodAstNode);

        peekedToken = parser->PeekNextToken();
    }

    Token rCurlyToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::rCurly, rCurlyToken))
        return nullptr;

    bodyAstNode->rightCurlyToken = rCurlyToken;

    return bodyAstNode;
}

std::shared_ptr<BodyAstNode> BodyParser::ParseMethodBody() const
{
    Token lCurlyToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::lCurly, lCurlyToken))
        return nullptr;

    auto bodyAstNode = std::make_shared<BodyAstNode>();
    bodyAstNode->leftCurlyToken = lCurlyToken;

    Token peekedToken = parser->PeekNextToken();

    while (peekedToken.kind != TokenKind::rCurly)
    {
        peekedToken = parser->PeekNextToken();
        if (peekedToken.kind == TokenKind::transmit)
        {
            const auto transmitAstNode = parser->transmitParser->Parse();
            if (transmitAstNode != nullptr)
                bodyAstNode->statementAstNodes.push_back(transmitAstNode);
        }
        else if (peekedToken.kind == TokenKind::identifier)
        {
            const Token nextToken = parser->PeekXForwardToken(1);
            if (nextToken.kind == TokenKind::identifier)
            {
                const auto variableDeclarationAstNode = parser->expressionParser->ParseExpression();
                if (variableDeclarationAstNode != nullptr)
                    bodyAstNode->statementAstNodes.push_back(variableDeclarationAstNode);
            }
            else if (nextToken.kind == TokenKind::lParen || nextToken.kind == TokenKind::dot)
            {
                const auto methodCallAstNode = parser->methodCallParser->Parse();
                if (methodCallAstNode != nullptr)
                    bodyAstNode->statementAstNodes.push_back(methodCallAstNode);
            }
            else
            {
                parser->GiveErrorByMessage(peekedToken, "Unexpected token while parsing method");
                break;
            }
        }
        else if (peekedToken.kind == TokenKind::rCurly)
        {
            break;
        }
        else
        {
            parser->GiveErrorByMessage(peekedToken, "Unexpected token while parsing method");
            break;
        }
    }

    Token rCurlyToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::rCurly, rCurlyToken))
        return nullptr;

    bodyAstNode->rightCurlyToken = rCurlyToken;

    return bodyAstNode;
}
