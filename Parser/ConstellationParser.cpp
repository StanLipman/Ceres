
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
        parser->GiveErrorByMessage(accessModifierToken, "Expected access modifier");
        return;
    }

    // TODO: Add handling for structs

    // TODO: Fix this: https://github.com/StanLipman/Ceres/issues/12
    const Token eternalToken = parser->GetNextToken();
    if (eternalToken.kind != TokenKind::eternal)
    {
        parser->GiveErrorByMessage(eternalToken, "All constellations should be eternal (at least for now)");
        return;
    }

    const Token constellationToken = parser->GetNextToken();
    if (constellationToken.kind != TokenKind::constellation)
    {
        parser->GiveError(constellationToken, TokenKind::constellation);
        return;
    }
    assert(constellation->token.kind == TokenKind::none);
    constellation->token = constellationToken;

    body->statementAstNodes.push_back(constellation);
    assert(constellation->accessModifier == TokenKind::none);
    constellation->accessModifier = accessModifierToken.kind;

    const Token constellationIdentifierToken = parser->GetNextToken();
    if (constellationIdentifierToken.kind != TokenKind::identifier)
    {
        parser->GiveError(constellationIdentifierToken, TokenKind::identifier);
        return;
    }

    assert(constellation->name.empty());
    constellation->name = constellationIdentifierToken.text;

    const Token leftCurlyToken = parser->GetNextToken();
    if (leftCurlyToken.kind != TokenKind::lCurly)
    {
        parser->GiveError(leftCurlyToken, TokenKind::lCurly);
        return;
    }

    const auto bodyAstNode = std::make_shared<BodyAstNode>();
    assert(constellation->bodyAstNode == nullptr);
    constellation->bodyAstNode = bodyAstNode;
    bodyAstNode->leftCurlyToken = leftCurlyToken;

    parser->bodyParser->Parse(constellation, bodyAstNode);

    const Token rightCurlyToken = parser->GetNextToken();
    if (rightCurlyToken.kind != TokenKind::rCurly)
    {
        parser->GiveError(rightCurlyToken, TokenKind::rCurly);
        return;
    }
    bodyAstNode->rightCurlyToken = rightCurlyToken;
}
