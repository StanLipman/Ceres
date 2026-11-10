#include "BodyParser.h"

#include "ParserManager.h"
#include "Models/Token.h"
#include "Models/Ast/ConstellationAstNode.h"
#include "Models/Ast/LiteralValueAstNode.h"
#include "Models/Ast/MethodAstNode.h"
#include "Models/Ast/TransmitAstNode.h"

class GalaxyAstNode;

void BodyParser::Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<BodyAstNode>& body)
{
    if (const auto galaxyAstNode = std::dynamic_pointer_cast<GalaxyAstNode>(parent))
    {
        const std::shared_ptr<ConstellationAstNode> constellationAstNode = std::make_shared<ConstellationAstNode>();
        // constellationAstNode->token is set in the constellationParser
        parser->constellationParser->Parse(body, constellationAstNode);
        return;
    }

    if (const auto constellationAstNode = std::dynamic_pointer_cast<ConstellationAstNode>(parent))
    {
        while (parser->PeekNextToken().kind != TokenKind::rCurly) // This works, because any other rCurly should already be consumed
        {
            // TODO: Handle fields

            const auto methodAstNode = std::make_shared<MethodAstNode>();
            // methodAstNode->token is set in methodParser
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

            const auto transmitAstNode = std::make_shared<TransmitAstNode>();
            transmitAstNode->token = nextToken;

            assert(methodAstNode->bodyAstNode != nullptr);
            methodAstNode->bodyAstNode->statementAstNodes.push_back(transmitAstNode);

            if (const Token peekToken = parser->PeekNextToken(); IsLiteralValueTokenKind(peekToken.kind))
            {
                assert(transmitAstNode->transmitValue == nullptr);

                if (const Token literalToken = parser->PeekNextToken(); literalToken.kind == TokenKind::_string || literalToken.kind == TokenKind::_char)
                {
                    auto stringAstNode = std::make_shared<LiteralStringValueAstNode>(literalToken);
                    parser->literalStringValueParser->Parse(transmitAstNode, stringAstNode);
                }
                else
                {
                    auto numericAstNode = std::make_shared<LiteralNumericValueAstNode>(literalToken);
                    parser->literalNumericValueParser->Parse(transmitAstNode, numericAstNode);
                }
            }

            if (const Token token = parser->GetNextToken(); token.kind != TokenKind::semicolon)
            {
                parser->GiveError(token, TokenKind::semicolon);
                return;
            }
        }
    }

    // assert(false); // TODO: Make handling
}
