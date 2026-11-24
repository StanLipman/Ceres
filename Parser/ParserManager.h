#pragma once
#include <memory>
#include <vector>

#include "BodyParser.h"
#include "ConstellationParser.h"
#include "DocumentParser.h"
#include "ExpressionParser.h"
#include "GalaxyParser.h"
#include "LiteralValueParser.h"
#include "MethodCallParser.h"
#include "MethodParser.h"
#include "TransmitParser.h"
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
    std::unique_ptr<DocumentParser> documentParser;
    std::unique_ptr<GalaxyParser> galaxyParser;
    std::unique_ptr<BodyParser> bodyParser;
    std::unique_ptr<ConstellationParser> constellationParser;
    std::unique_ptr<MethodParser> methodParser;
    std::unique_ptr<LiteralValueParser> literalValueParser;
    std::unique_ptr<TransmitParser> transmitParser;
    std::unique_ptr<MethodCallParser> methodCallParser;
    std::unique_ptr<ExpressionParser> expressionParser;

    explicit ParseManager(CompilerContext* compilerContext, const std::vector<Token>& tokens)
    {
        this->compilerContext = compilerContext;

        this->tokens = tokens;

        this->documentParser = std::make_unique<DocumentParser>(this);
        this->galaxyParser = std::make_unique<GalaxyParser>(this);
        this->bodyParser = std::make_unique<BodyParser>(this);
        this->constellationParser = std::make_unique<ConstellationParser>(this);
        this->methodParser = std::make_unique<MethodParser>(this);
        this->literalValueParser = std::make_unique<LiteralValueParser>(this);
        this->transmitParser = std::make_unique<TransmitParser>(this);
        this->methodCallParser = std::make_unique<MethodCallParser>(this);
        this->expressionParser = std::make_unique<ExpressionParser>(this);
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

    Token PeekXForwardToken(uint amountForward)
    {
        if (tokens.size() < currentTokenIndex)
            return tokens.back();

        return tokens[currentTokenIndex + amountForward];
    }

    void BuildAst()
    {
        this->documentAstNode = documentParser->Parse();
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

    bool TryConsumeOnMatchOrSkip(const TokenKind expectedTokenKind)
    {
        const Token nextToken = PeekNextToken();
        if (nextToken.kind != expectedTokenKind)
            return false;

        (void) GetNextToken();
        return true;
    }

    bool TryConsumeOrError(const TokenKind expectedTokenKind)
    {
        const Token nextToken = GetNextToken();
        if (nextToken.kind != expectedTokenKind)
        {
            GiveError(nextToken, expectedTokenKind);
            return false;
        }

        return true;
    }

    bool TryGetNextTokenOnMatchOrSkip(const TokenKind expectedTokenKind, Token& outToken)
    {
        const Token nextToken = PeekNextToken();
        if (nextToken.kind != expectedTokenKind)
        {
            outToken = EmptyToken;
            return false;
        }

        outToken = GetNextToken();
        assert(outToken.kind == expectedTokenKind);
        return true;
    }

    bool TryGetNextTokenOrError(const TokenKind expectedTokenKind, Token& outToken)
    {
        const Token nextToken = GetNextToken();
        if (nextToken.kind != expectedTokenKind)
        {
            outToken = EmptyToken;
            GiveError(nextToken, expectedTokenKind);
            return false;
        }

        outToken = nextToken;
        return true;
    }

    bool TryGetNextTokenOrError(const TokenKind expectedTokenKind, const std::string& errorMessage, Token& outToken)
    {
        const Token nextToken = GetNextToken();
        if (nextToken.kind != expectedTokenKind)
        {
            outToken = EmptyToken;
            GiveErrorByMessage(nextToken, errorMessage);
            return false;
        }

        outToken = nextToken;
        return true;
    }
};
