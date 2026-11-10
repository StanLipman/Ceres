#include "GalaxyParser.h"
#include "ParserManager.h"
#include "Models/Token.h"

void GalaxyParser::Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<GalaxyAstNode>& galaxy)
{
    assert(parent == nullptr);

    const Token identifierToken = parser->GetNextToken();

    if (identifierToken.kind != TokenKind::identifier)
    {
        parser->GiveError(identifierToken, TokenKind::identifier);
        return;
    }

    assert(galaxy->name.empty());
    galaxy->name = identifierToken.text;

    const Token leftCurlyToken = parser->GetNextToken();
    if (leftCurlyToken.kind != TokenKind::lCurly)
    {
        parser->GiveError(leftCurlyToken, TokenKind::lCurly);
        return;
    }

    const auto bodyAstNode = std::make_shared<BodyAstNode>();
    bodyAstNode->leftCurlyToken = leftCurlyToken;

    assert(galaxy->bodyAstNode == nullptr);
    galaxy->bodyAstNode = bodyAstNode;

    parser->bodyParser->Parse(galaxy, bodyAstNode);

    if (const Token rightCurlyToken = parser->GetNextToken(); rightCurlyToken.kind != TokenKind::rCurly)
    {
        parser->GiveError(rightCurlyToken, TokenKind::rCurly);
        return;
    }
    else
    {
        bodyAstNode->rightCurlyToken = rightCurlyToken;
    }
}
