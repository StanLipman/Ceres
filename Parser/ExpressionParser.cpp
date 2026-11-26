#include "ExpressionParser.h"

#include "ParserManager.h"
#include "Models/Token.h"
#include "Models/Ast/AutoAstNode.h"
#include "Models/Ast/VariableAstNode.h"

std::shared_ptr<ExpressionAstNode> ExpressionParser::ParseVariableDeclarationExpression() const
{
    std::shared_ptr<AstNode> typeOrAutoAstNode;
    if (Token typeIdentifierToken; parser->TryGetNextTokenOnMatchOrSkip(TokenKind::identifier, typeIdentifierToken))
    {
        const auto typeAstNode = std::make_shared<TypeAstNode>();
        typeAstNode->token = typeIdentifierToken;
        typeAstNode->name = typeIdentifierToken.text;

        typeOrAutoAstNode = typeAstNode;
    }
    else
    {
        Token autoToken;
        if (!parser->TryGetNextTokenOrError(TokenKind::_auto, autoToken))
            return nullptr;

        const auto autoAstNode = std::make_shared<AutoAstNode>();
        autoAstNode->token = autoToken;

        typeOrAutoAstNode = autoAstNode;
    }

    Token variableNameIdentifierToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::identifier, variableNameIdentifierToken))
        return nullptr;

    auto variableDeclarationAstNode = std::make_shared<VariableDeclarationAstNode>();
    variableDeclarationAstNode->name = variableNameIdentifierToken.text;
    variableDeclarationAstNode->token = variableNameIdentifierToken;
    variableDeclarationAstNode->type = typeOrAutoAstNode;

    Token assignToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::assign, assignToken))
        return nullptr; // TODO: Add support for uninitialized vars that get assigned later

    variableDeclarationAstNode->initializer = ParseAssignValueAstNode();
    assert(variableDeclarationAstNode->initializer !=  nullptr);

    if (!parser->TryConsumeOrError(TokenKind::semicolon))
        return nullptr;

    return variableDeclarationAstNode;
}

std::shared_ptr<ExpressionAstNode> ExpressionParser::ParseVariableAssignmentExpression() const
{
    Token variableNameIdentifierToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::identifier, variableNameIdentifierToken))
        return nullptr;

    if (!parser->TryConsumeOrError(TokenKind::assign))
        return nullptr;

    auto assignAstNode = std::make_shared<AssignExpressionAstNode>();
    assignAstNode->token = variableNameIdentifierToken;

    const auto variableAstNode = std::make_shared<VariableAstNode>();
    variableAstNode->name = variableNameIdentifierToken.text;
    variableAstNode->name = variableNameIdentifierToken.text;

    assignAstNode->target = variableAstNode;
    assignAstNode->value = ParseAssignValueAstNode();
    assert(assignAstNode->value != nullptr);

    if (!parser->TryConsumeOrError(TokenKind::semicolon))
        return nullptr;

    return assignAstNode;
}

std::shared_ptr<AstNode> ExpressionParser::ParseAssignValueAstNode() const
{
    if (parser->PeekNextToken().kind == TokenKind::identifier)
    {
        if (parser->PeekXForwardToken(1).kind == TokenKind::semicolon) // If an identifier is followed by an semicolon it is a variable
        {
            const auto variableIdentifierToken = parser->GetNextToken();
            const auto variableAstNode = std::make_shared<VariableAstNode>();
            variableAstNode->token = variableIdentifierToken;
            variableAstNode->name = variableIdentifierToken.text;

            return variableAstNode;
        }

        return parser->methodCallParser->ParseExpression();
    }

    assert(IsLiteralValueTokenKind(parser->PeekNextToken().kind));
    return parser->literalValueParser->Parse();
}
