#include "ExpressionParser.h"

#include "ParserManager.h"
#include "Models/Token.h"
#include "Models/Ast/AutoAstNode.h"
#include "Models/Ast/VariableAstNode.h"

std::shared_ptr<ExpressionAstNode> ExpressionParser::ParseExpression() const
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
        return nullptr;

    const auto assignExpressionAstNode = std::make_shared<AssignExpressionAstNode>();
    assignExpressionAstNode->token = assignToken;

    variableDeclarationAstNode->assignmentExpression = assignExpressionAstNode;

    if (parser->PeekNextToken().kind == TokenKind::identifier)
    {
        if (parser->PeekXForwardToken(1).kind == TokenKind::semicolon) // If an identifier is followed by an semicolon it is a variable
        {
            const auto variableIdentifierToken = parser->GetNextToken();
            const auto variableAstNode = std::make_shared<VariableAstNode>();
            variableAstNode->token = variableIdentifierToken;
            variableAstNode->name = variableIdentifierToken.text;

            assignExpressionAstNode->assignee = variableAstNode;

            auto result = parser->TryConsumeOrError(TokenKind::semicolon);
            assert(result);

            return variableDeclarationAstNode;
        }

        const auto& methodCallAstNode = parser->methodCallParser->Parse();
        if (methodCallAstNode != nullptr)
        {
            assignExpressionAstNode->assignee = methodCallAstNode;
            return variableDeclarationAstNode; // The semicolon is handled by the method call parsing
        }
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
