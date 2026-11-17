#pragma once
#include "Models/Ast/ConstellationAstNode.h"

class ParseManager;

class ConstellationParser
{
private:
    ParseManager* parser;
public:
    explicit ConstellationParser(ParseManager* parser) : parser(parser) { }

    [[nodiscard]] std::shared_ptr<ConstellationAstNode> Parse() const;
};
