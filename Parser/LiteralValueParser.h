#pragma once
#include <memory>

#include "Models/Ast/LiteralValueAstNode.h"

class ParseManager;

class LiteralValueParser
{
private:
    ParseManager* parser;
    [[nodiscard]] std::shared_ptr<LiteralValueAstNode> ParseStringLiteral(const Token& literalStringToken) const;
    [[nodiscard]] static std::shared_ptr<LiteralValueAstNode> ParseNumericLiteral(const Token& literalToken);

public:
    explicit LiteralValueParser(ParseManager* parser) : parser(parser) { }

    [[nodiscard]] std::shared_ptr<LiteralValueAstNode> Parse() const;
};
