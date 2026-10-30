#include "BodyParser.h"

#include "ParserManager.h"
#include "Models/Token.h"
#include "Models/Ast/ConstellationAstNode.h"
#include "Models/Ast/MethodAstNode.h"

class GalaxyAstNode;

void BodyParser::Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<BodyAstNode>& body)
{
    if (const auto galaxyAstNode = std::dynamic_pointer_cast<GalaxyAstNode>(parent))
    {
        const std::shared_ptr<ConstellationAstNode> constellationAstNode = std::make_shared<ConstellationAstNode>();
        parser->constellationParser->Parse(body, constellationAstNode);
        return;
    }

    if (const auto constellationAstNode = std::dynamic_pointer_cast<ConstellationAstNode>(parent))
    {
        while (parser->PeekNextToken().kind != TokenKind::rCurly) // This works, because any other rCurly should already be consumed
        {
            // TODO: Handle fields

            const auto methodAstNode = std::make_shared<MethodAstNode>();
            parser->methodParser->Parse(body, methodAstNode);
        }
        return;
    }

    // assert(false); // TODO: Make handling
}
