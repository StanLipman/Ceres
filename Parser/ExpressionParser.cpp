#include "ExpressionParser.h"

#include "ParserManager.h"
#include "Models/Token.h"
#include "Models/Ast/VariableAstNode.h"

std::shared_ptr<ExpressionAstNode> ExpressionParser::ParseExpression() const
{
    // This is currently only assuming variable declaration with assignment
    Token typeIdentifierToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::identifier, typeIdentifierToken))
        return nullptr;

    Token variableNameIdentifierToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::identifier, variableNameIdentifierToken))
        return nullptr;

    auto variableDeclarationAstNode = std::make_shared<VariableDeclarationAstNode>();
    variableDeclarationAstNode->name = variableNameIdentifierToken.text;
    variableDeclarationAstNode->token = variableNameIdentifierToken;
    variableDeclarationAstNode->typeToken = typeIdentifierToken;

    // TODO: Allow declaration only, no assignment
    Token assignToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::assign, assignToken))
        return nullptr;

    const auto assignExpressionAstNode = std::make_shared<AssignExpressionAstNode>();
    assignExpressionAstNode->token = assignToken;

    variableDeclarationAstNode->assignmentExpression = assignExpressionAstNode;

    if (parser->PeekNextToken().kind == TokenKind::identifier)
    {
        const auto variableToken = parser->GetNextToken();
        const auto variableAstNode = std::make_shared<VariableAstNode>();
        variableAstNode->token = variableToken;
        variableAstNode->name = variableToken.text;

        assignExpressionAstNode->assignee = variableAstNode;
    }
    else
    {
        assert(IsLiteralValueTokenKind(parser->PeekNextToken().kind));

        const auto literalValueAstNode = parser->literalValueParser->Parse();
        if (literalValueAstNode != nullptr)
            assignExpressionAstNode->assignee = literalValueAstNode;
    }

    if (!parser->TryConsumeOrError(TokenKind::semicolon))
        return nullptr;

    return variableDeclarationAstNode;
}
