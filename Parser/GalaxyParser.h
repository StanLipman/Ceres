#pragma once
#include "Models/Ast/GalaxyAstNode.h"

class ParseManager;

class GalaxyParser
{
private:
    ParseManager* parser;

public:
    explicit GalaxyParser(ParseManager* parser) : parser(parser) { }

    [[nodiscard]] std::shared_ptr<GalaxyAstNode> Parse() const;
};
