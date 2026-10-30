#pragma once
#include <memory>
#include <stack>
#include <vector>

#include "BodyParser.h"
#include "ConstellationParser.h"
#include "DocumentParser.h"
#include "GalaxyParser.h"
#include "MethodParser.h"
#include "Models/Token.h"
#include "Models/Ast/DocumentAstNode.h"

class ParseManager
{
private:
    std::vector<Token> tokens;

    std::shared_ptr<DocumentAstNode> documentAstNode;
    size_t currentTokenIndex = 0;

public:
    std::shared_ptr<DocumentParser> documentParser;
    std::shared_ptr<GalaxyParser> galaxyParser;
    std::shared_ptr<BodyParser> bodyParser;
    std::shared_ptr<ConstellationParser> constellationParser;
    std::shared_ptr<MethodParser> methodParser;

    explicit ParseManager(const std::vector<Token>& tokens)
    {
        this->tokens = tokens;

        this->documentParser = std::make_shared<DocumentParser>(this);
        this->galaxyParser = std::make_shared<GalaxyParser>(this);
        this->bodyParser = std::make_shared<BodyParser>(this);
        this->constellationParser = std::make_shared<ConstellationParser>(this);
        this->methodParser = std::make_shared<MethodParser>(this);
    }

    Token GetNextToken()
    {
        if (tokens.size() < currentTokenIndex)
            return tokens.back();

        return tokens[currentTokenIndex++];
    }

    Token PeekNextToken()
    {
        if (tokens.size() < currentTokenIndex)
            return tokens.back();

        return tokens[currentTokenIndex + 1];
    }

    void BuildAst()
    {
        this->documentAstNode = std::make_shared<DocumentAstNode>();
        documentParser->Parse(nullptr, this->documentAstNode);

        this->documentAstNode->Print(0);
    }

    static void GiveError(const Token& token, const std::string& message)
    {
        assert(false);
        printf("Syntax error at %lu:%lu.\n\tmessage: %s. Got %s\n", token.location.line, token.location.column, message.c_str(), GetTokenKindText(token.kind).c_str());
    }
};
