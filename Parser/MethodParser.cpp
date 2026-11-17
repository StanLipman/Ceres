#include "MethodParser.h"
#include "ParserManager.h"

std::shared_ptr<MethodAstNode> MethodParser::Parse() const
{
    const Token accessModifierToken = parser->GetNextToken();
    if (!IsAccessModifier(accessModifierToken.kind))
    {
        parser->GiveErrorByMessage(accessModifierToken, "Expected access modifier");
        return nullptr;
    }

    // TODO: Fix this: https://github.com/StanLipman/Ceres/issues/12
    if (!parser->TryConsumeOrError(TokenKind::eternal))
        return nullptr;

    const Token returnTypeToken = parser->GetNextToken();
    if (returnTypeToken.kind != TokenKind::identifier && returnTypeToken.kind != TokenKind::vacuum)
    {
        parser->GiveError(returnTypeToken, TokenKind::identifier);
        return nullptr;
    }

    const auto returnTypeAstNode = std::make_shared<TypeAstNode>();
    returnTypeAstNode->token = returnTypeToken;
    returnTypeAstNode->name = returnTypeToken.text;

    Token identifierToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::identifier, identifierToken))
        return nullptr;

    auto methodAstNode = std::make_shared<MethodAstNode>();
    methodAstNode->token = identifierToken;
    methodAstNode->name = identifierToken.text;
    methodAstNode->accessModifier = accessModifierToken.kind;
    methodAstNode->isEternal = true; // TODO: Fix this: https://github.com/StanLipman/Ceres/issues/12
    methodAstNode->returnTypeAstNode = returnTypeAstNode;

    if (!parser->TryConsumeOrError(TokenKind::lParen))
        return nullptr;

    while (parser->PeekNextToken().kind != TokenKind::rParen)
    {
        const auto argumentAstNode = ParseArgument();
        if (argumentAstNode != nullptr)
            methodAstNode->argumentAstNodes.push_back(argumentAstNode);
    }

    if (Token rightParenToken; !parser->TryGetNextTokenOrError(TokenKind::rParen, rightParenToken))
        return nullptr;

    const auto bodyAstNode = parser->bodyParser->ParseMethodBody();
    if (bodyAstNode == nullptr)
        return nullptr;

    methodAstNode->bodyAstNode = bodyAstNode;

    return methodAstNode;
}

std::shared_ptr<ArgumentAstNode> MethodParser::ParseArgument() const
{
    if (parser->TryConsumeOnMatchOrSkip(TokenKind::comma))
        return nullptr;

    Token argumentTypeToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::identifier, argumentTypeToken))
        return nullptr;

    const auto typeAstNode = std::make_shared<TypeAstNode>();
    typeAstNode->token = argumentTypeToken;
    typeAstNode->name = argumentTypeToken.text;

    auto argumentAstNode = std::make_shared<ArgumentAstNode>();
    argumentAstNode->typeAstNode = typeAstNode;

    Token argumentNameToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::identifier, argumentNameToken))
        return nullptr;

    argumentAstNode->name = argumentNameToken.text;

    return argumentAstNode;
}
