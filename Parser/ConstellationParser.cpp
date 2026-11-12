
#include "ConstellationParser.h"
#include "ParserManager.h"

std::shared_ptr<ConstellationAstNode> ConstellationParser::Parse() const
{
    const Token accessModifierToken = parser->GetNextToken();
    if (!IsAccessModifier(accessModifierToken.kind))
    {
        parser->GiveErrorByMessage(accessModifierToken, "Expected access modifier");
        return nullptr;
    }

    // TODO: Fix this: https://github.com/StanLipman/Ceres/issues/12
    if (Token eternalToken; !parser->TryGetNextTokenOrError(TokenKind::eternal, "All constellations should be eternal (at least for now)", eternalToken))
        return nullptr;

    Token constellationToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::constellation, constellationToken))
        return nullptr;

    Token identifierToken;
    if (!parser->TryGetNextTokenOrError(TokenKind::identifier, identifierToken))
        return nullptr;

    auto constellationAstNode = std::make_shared<ConstellationAstNode>();
    constellationAstNode->token = constellationToken;
    constellationAstNode->name = identifierToken.text;
    constellationAstNode->accessModifier = accessModifierToken.kind;
    constellationAstNode->isEternal = true; // TODO: Fix this: https://github.com/StanLipman/Ceres/issues/12

    const auto bodyAstNode = parser->bodyParser->ParseConstellationBody();
    constellationAstNode->bodyAstNode = bodyAstNode;

    return constellationAstNode;
}
