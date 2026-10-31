
#include "ConstellationParser.h"
#include "ParserManager.h"

void ConstellationParser::Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<ConstellationAstNode>& constellation)
{
    assert(parent != nullptr);
    const std::shared_ptr<BodyAstNode> body = std::dynamic_pointer_cast<BodyAstNode>(parent);
    if (body == nullptr)
    {
        assert(false); // How did we get anything different than a body?
        return;
    }

    const Token accessModifierToken = parser->GetNextToken();
    if (!IsAccessModifier(accessModifierToken.kind))
    {
        ParseManager::GiveError(accessModifierToken, "Expected access modifier");
        return;
    }

    // TODO: Add handling for eternal
    // TODO: Add handling for structs
    const Token constellationToken = parser->GetNextToken();
    if (constellationToken.kind != TokenKind::constellation)
    {
        ParseManager::GiveError(constellationToken, "Expected access constellation");
        return;
    }

    body->AddStatement(constellation);
    constellation->SetAccessModifier(accessModifierToken.kind);

    const Token constellationIdentifierToken = parser->GetNextToken();
    if (constellationIdentifierToken.kind != TokenKind::identifier)
    {
        ParseManager::GiveError(constellationIdentifierToken, "Expected identifier");
        return;
    }

    constellation->SetName(constellationIdentifierToken.text);

    const Token leftCurlyToken = parser->GetNextToken();
    if (leftCurlyToken.kind != TokenKind::lCurly)
    {
        ParseManager::GiveError(leftCurlyToken, "Expected left curly");
        return;
    }

    const auto bodyAstNode = std::make_shared<BodyAstNode>();
    constellation->SetBody(bodyAstNode);
    parser->bodyParser->Parse(constellation, bodyAstNode);

    if (const Token rightCurlyToken = parser->GetNextToken(); rightCurlyToken.kind != TokenKind::rCurly)
    {
        ParseManager::GiveError(rightCurlyToken, "Expected right curly");
        return;
    }
}
