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
        if (peekedToken.kind == TokenKind::transmit)
        {
            const auto transmitAstNode = parser->transmitParser->Parse();
            if (transmitAstNode != nullptr)
                bodyAstNode->statementAstNodes.push_back(transmitAstNode);
        }
        else if (peekedToken.kind == TokenKind::identifier)
        {
            const auto methodCallAstNode = parser->methodCallParser->Parse();
            if (methodCallAstNode != nullptr)
                bodyAstNode->statementAstNodes.push_back(methodCallAstNode);
        }

        peekedToken = parser->PeekNextToken();
    }

    Token rCurlyToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::rCurly, rCurlyToken))
        return nullptr;

    bodyAstNode->rightCurlyToken = rCurlyToken;

    return bodyAstNode;
}
