#include "BodyParser.h"

#include "ParserManager.h"
#include "Models/Token.h"
#include "Models/Ast/ConstellationAstNode.h"
#include "Models/Ast/MethodAstNode.h"
#include "Models/Ast/TransmitAstNode.h"

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

    if (const auto methodAstNode = std::dynamic_pointer_cast<MethodAstNode>(parent))
    {
        const Token nextToken = parser->PeekNextToken();
        if (nextToken.kind == TokenKind::transmit)
        {
            const Token transmitToken = parser->GetNextToken();
            if (const Token token = parser->GetNextToken(); token.kind != TokenKind::semicolon)
            {
                ParseManager::GiveError(token, "Expected semicolon");
                return;
            }

            const auto transmitAstNode = std::make_shared<TransmitAstNode>();
            assert(methodAstNode->bodyAstNode != nullptr);
            methodAstNode->bodyAstNode->statementAstNodes.push_back(transmitAstNode);
        }
    }

    // assert(false); // TODO: Make handling
}
