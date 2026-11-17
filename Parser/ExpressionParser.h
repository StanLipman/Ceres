#pragma once
#include <memory>

#include "Models/Ast/ExpressionAstNode.h"

class ParseManager;

class ExpressionParser
{
private:
    ParseManager* parser;

public:
    explicit ExpressionParser(ParseManager* parser): parser(parser) {}

    [[nodiscard]] std::shared_ptr<ExpressionAstNode> ParseExpression() const;
};
