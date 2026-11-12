#include "DocumentParser.h"
#include "ParserManager.h"

std::shared_ptr<DocumentAstNode> DocumentParser::Parse() const
{
    auto documentAstNode = std::make_shared<DocumentAstNode>();

    while (true)
    {
        if (parser->TryConsumeOnMatchOrSkip(TokenKind::eof))
            break;

        if (const Token nextToken = parser->PeekNextToken(); nextToken.kind != TokenKind::galaxy)
        {
            parser->GiveError(nextToken, TokenKind::galaxy);
            return nullptr;
        }

        const auto galaxyAstNode = parser->galaxyParser->Parse();
        if (galaxyAstNode != nullptr)
            documentAstNode->galaxyAstNodes.push_back(galaxyAstNode);
    }

    return documentAstNode;
}
