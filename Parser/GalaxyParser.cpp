#include "GalaxyParser.h"
#include "ParserManager.h"
#include "Models/Token.h"

std::shared_ptr<GalaxyAstNode> GalaxyParser::Parse() const
{
    const Token galaxyToken = parser->GetNextToken();
    assert(galaxyToken.kind == TokenKind::galaxy);

    Token identifierToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::identifier, identifierToken))
        return nullptr;

    const auto bodyAstNode = parser->bodyParser->ParseGalaxyBody();
    if (bodyAstNode == nullptr)
        return nullptr;

    auto galaxyAstNode = std::make_shared<GalaxyAstNode>();
    galaxyAstNode->token = galaxyToken;
    galaxyAstNode->name = identifierToken.text;
    galaxyAstNode->bodyAstNode = bodyAstNode;

    return galaxyAstNode;
}
