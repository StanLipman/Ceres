#include "GalaxyParser.h"
#include "ParserManager.h"
#include "Models/Token.h"

void GalaxyParser::Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<GalaxyAstNode>& galaxy)
{
    assert(parent == nullptr);

    const Token identifierToken = parser->GetNextToken();

    if (identifierToken.kind != TokenKind::identifier)
    {
        ParseManager::GiveError(identifierToken, "Expected identifier");
        return;
    }

    galaxy->SetName(identifierToken.text);

    const Token leftCurlyToken = parser->GetNextToken();
    if (leftCurlyToken.kind != TokenKind::lCurly)
    {
        ParseManager::GiveError(leftCurlyToken, "Expected left curly");
        return;
    }

    const auto bodyAstNode = std::make_shared<BodyAstNode>();
    galaxy->SetBody(bodyAstNode);
    parser->bodyParser->Parse(galaxy, bodyAstNode);

    if (const Token rightCurlyToken = parser->GetNextToken(); rightCurlyToken.kind != TokenKind::rCurly)
    {
        ParseManager::GiveError(rightCurlyToken, "Expected right curly");
        return;
    }
}
