
#include "MethodParser.h"

#include "ParserManager.h"

void MethodParser::Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<MethodAstNode>& method)
{
    assert(parent != nullptr);
    const std::shared_ptr<BodyAstNode> body = std::dynamic_pointer_cast<BodyAstNode>(parent);
    if (body == nullptr)
    {
        assert(false); // How did we get anything different than a body?
        return;
    }

    body->AddStatement(method);

    const Token accessModifierToken = parser->GetNextToken();
    if (!IsAccessModifier(accessModifierToken.kind))
    {
        ParseManager::GiveError(accessModifierToken, "Expected access modifier");
        return;
    }

    method->SetAccessModifier(accessModifierToken.kind);

    // TODO: Add handling for eternal
    const Token returnTypeToken = parser->GetNextToken();
    if (returnTypeToken.kind != TokenKind::vacuum)
    {
        ParseManager::GiveError(returnTypeToken, "Only vacuum is accepted as return type");
        return;
    }
    const auto returnTypeAstNode = std::make_shared<TypeAstNode>();
    returnTypeAstNode->SetName(returnTypeToken.text);

    method->SetReturnType(returnTypeAstNode);

    const Token identifierToken = parser->GetNextToken();
    if (identifierToken.kind != TokenKind::identifier)
    {
        ParseManager::GiveError(identifierToken, "Expected identifier");
        return;
    }

    method->SetName(identifierToken.text);

    const Token leftParenToken = parser->GetNextToken();
    if (leftParenToken.kind != TokenKind::lParen)
    {
        ParseManager::GiveError(leftParenToken, "Expected left parenthesis");
        return;
    }

    // TODO: parse arguments
    const Token rightParenToken = parser->GetNextToken();
    if (rightParenToken.kind != TokenKind::rParen)
    {
        ParseManager::GiveError(rightParenToken, "Expected right parenthesis");
        return;
    }

    const Token leftCurlyToken = parser->GetNextToken();
    if (leftCurlyToken.kind != TokenKind::lCurly)
    {
        ParseManager::GiveError(leftCurlyToken, "Expected left curly");
        return;
    }

    const auto bodyAstNode = std::make_shared<BodyAstNode>();
    method->SetBody(bodyAstNode);
    parser->bodyParser->Parse(method, bodyAstNode);

    const Token rightCurlyToken = parser->GetNextToken();
    if (rightCurlyToken.kind != TokenKind::rCurly)
    {
        ParseManager::GiveError(rightCurlyToken, "Expected right curly");
        return;
    }
}
