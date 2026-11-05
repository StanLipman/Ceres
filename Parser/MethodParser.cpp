
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

    body->statementAstNodes.push_back(method);

    const Token accessModifierToken = parser->GetNextToken();
    if (!IsAccessModifier(accessModifierToken.kind))
    {
        ParseManager::GiveError(accessModifierToken, "Expected access modifier");
        return;
    }

    assert(method->accessModifier == TokenKind::none);
    method->accessModifier = accessModifierToken.kind;

    const Token eternalToken = parser->GetNextToken();
    if (eternalToken.kind != TokenKind::eternal)
    {
        ParseManager::GiveError(eternalToken, "All methods should be eternal (at least for now)");
        return;
    }
    // TODO: Re-enable this when we make eternal optional again on methods
    // if (parser->PeekNextToken().kind == TokenKind::eternal)
    // {
    //     const Token eternalToken = parser->GetNextToken();
    //     assert(eternalToken.kind == TokenKind::eternal);
    //     method->SetIsEternal();
    // }

    const Token returnTypeToken = parser->GetNextToken();
    if (returnTypeToken.kind != TokenKind::identifier && returnTypeToken.kind != TokenKind::vacuum)
    {
        ParseManager::GiveError(returnTypeToken, "Expected return type");
        return;
    }

    const auto primitiveReturnType = std::make_shared<TypeAstNode>();
    assert(primitiveReturnType->name.empty());
    primitiveReturnType->name = returnTypeToken.text;

    assert(method->returnTypeAstNode == nullptr);
    method->returnTypeAstNode = primitiveReturnType;

    const Token identifierToken = parser->GetNextToken();
    if (identifierToken.kind != TokenKind::identifier)
    {
        ParseManager::GiveError(identifierToken, "Expected identifier");
        return;
    }

    assert(method->name.empty());
    method->name = identifierToken.text;

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
    assert(method->bodyAstNode == nullptr);
    method->bodyAstNode = bodyAstNode;
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
    method->argumentAstNodes.push_back(argumentAstNode);

    const Token argumentTypeToken = parser->GetNextToken();
    if (argumentTypeToken.kind != TokenKind::identifier)
    {
        ParseManager::GiveError(argumentTypeToken, "Expected type identifier");
        return;
    }

    const std::shared_ptr<TypeAstNode> typeAstNode = std::make_shared<TypeAstNode>();
    assert(typeAstNode->name.empty());
    typeAstNode->name = argumentTypeToken.text;

    assert(argumentAstNode->typeAstNode == nullptr);
    argumentAstNode->typeAstNode = typeAstNode;

    const Token argumentNameToken = parser->GetNextToken();
    if (argumentNameToken.kind != TokenKind::identifier)
    {
        ParseManager::GiveError(argumentNameToken, "Expected name identifier");
        return;
    }

    assert(argumentAstNode->name.empty());
    argumentAstNode->name = argumentNameToken.text;
}
