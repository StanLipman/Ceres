#pragma once
#include <memory>
#include <vector>
#include <format>

#include "BodyParser.h"
#include "ConstellationParser.h"
#include "DocumentParser.h"
#include "GalaxyParser.h"
#include "LiteralValueParser.h"
#include "MethodParser.h"
#include "Models/Token.h"
#include "Models/Ast/DocumentAstNode.h"

class ParseManager
{
private:
    std::vector<Token> tokens;

    std::shared_ptr<DocumentAstNode> documentAstNode;
    size_t currentTokenIndex = 0;

    CompilerContext* compilerContext;

public:
    std::shared_ptr<DocumentParser> documentParser;
    std::shared_ptr<GalaxyParser> galaxyParser;
    std::shared_ptr<BodyParser> bodyParser;
    std::shared_ptr<ConstellationParser> constellationParser;
    std::shared_ptr<MethodParser> methodParser;
    std::shared_ptr<LiteralStringValueParser> literalStringValueParser;
    std::shared_ptr<LiteralNumericValueParser> literalNumericValueParser;

    explicit ParseManager(CompilerContext* compilerContext, const std::vector<Token>& tokens)
    {
        this->compilerContext = compilerContext;

        this->tokens = tokens;

        this->documentParser = std::make_shared<DocumentParser>(this);
        this->galaxyParser = std::make_shared<GalaxyParser>(this);
        this->bodyParser = std::make_shared<BodyParser>(this);
        this->constellationParser = std::make_shared<ConstellationParser>(this);
        this->methodParser = std::make_shared<MethodParser>(this);
        this->literalStringValueParser = std::make_shared<LiteralStringValueParser>(this);
        this->literalNumericValueParser = std::make_shared<LiteralNumericValueParser>(this);
    }

    std::shared_ptr<DocumentAstNode> GetDocumentAstNode()
    {
        return this->documentAstNode;
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

        return tokens[currentTokenIndex];
    }

    void BuildAst()
    {
        this->documentAstNode = std::make_shared<DocumentAstNode>();
        documentParser->Parse(nullptr, this->documentAstNode);
    }

    void GiveError(const Token& token, const TokenKind expectedTokenKind) const
    {
        const std::string formattedMessage = "Expected '" + GetTokenKindText(expectedTokenKind) + "' but got '" + GetTokenKindText(token.kind) + "'";
        GiveErrorByMessage(token, formattedMessage);
    }

    void GiveErrorByMessage(const Token& token, const std::string& message) const
    {
        compilerContext->compilationErrors.emplace_back(message, token.location);
    }
};
