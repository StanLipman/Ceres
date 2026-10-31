
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

    if (parser->PeekNextToken().kind == TokenKind::eternal)
    {
        const Token eternalToken = parser->GetNextToken();
        assert(eternalToken.kind == TokenKind::eternal);
        method->SetIsEternal();
    }

    const Token returnTypeToken = parser->GetNextToken();
    if (returnTypeToken.kind != TokenKind::identifier && returnTypeToken.kind != TokenKind::vacuum)
    {
        ParseManager::GiveError(returnTypeToken, "Expected return type");
        return;
    }

    const auto primitiveReturnType = std::make_shared<TypeAstNode>();
    primitiveReturnType->SetName(returnTypeToken.text);
    method->SetReturnType(primitiveReturnType);

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

    while (parser->PeekNextToken().kind != TokenKind::rParen)
        ParseArgument(method);

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

void MethodParser::ParseArgument(const std::shared_ptr<MethodAstNode>& method)
{
    if (parser->PeekNextToken().kind == TokenKind::comma)
        parser->GetNextToken();

    const std::shared_ptr<ArgumentAstNode> argumentAstNode = std::make_shared<ArgumentAstNode>();
    method->AddArgument(argumentAstNode);

    const Token argumentTypeToken = parser->GetNextToken();
    if (argumentTypeToken.kind != TokenKind::identifier)
    {
        ParseManager::GiveError(argumentTypeToken, "Expected type identifier");
        return;
    }

    const std::shared_ptr<TypeAstNode> typeAstNode = std::make_shared<TypeAstNode>();
    typeAstNode->SetName(argumentTypeToken.text);
    argumentAstNode->SetType(typeAstNode);

    const Token argumentNameToken = parser->GetNextToken();
    if (argumentNameToken.kind != TokenKind::identifier)
    {
        ParseManager::GiveError(argumentNameToken, "Expected name identifier");
        return;
    }

    argumentAstNode->SetName(argumentNameToken.text);
}
