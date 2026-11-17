#pragma once
#include "Models/Ast/MethodAstNode.h"

class ParseManager;

class MethodParser
{
private:
    ParseManager* parser;

public:
    explicit MethodParser(ParseManager* parser) : parser(parser) { }

    [[nodiscard]] std::shared_ptr<MethodAstNode> Parse() const;
    [[nodiscard]] std::shared_ptr<ArgumentAstNode> ParseArgument() const;
};
