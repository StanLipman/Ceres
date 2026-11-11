#include "DocumentParser.h"

#include "ParserManager.h"

void DocumentParser::Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<DocumentAstNode>& document)
{
    while (true)
    {
        Token nextToken = parser->GetNextToken();
        if (nextToken.kind == TokenKind::eof)
            break;

        if (nextToken.kind != TokenKind::galaxy)
        {
            parser->GiveError(nextToken, TokenKind::galaxy);
            return;
        }

        const auto galaxyAstNode = std::make_shared<GalaxyAstNode>();
        galaxyAstNode->token = nextToken;
        document->galaxyAstNodes.push_back(galaxyAstNode);

        parser->galaxyParser->Parse(nullptr, galaxyAstNode);
    }
}
