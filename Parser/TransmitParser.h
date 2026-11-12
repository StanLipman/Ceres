#pragma once
#include "Models/Ast/TransmitAstNode.h"

class ParseManager;

class TransmitParser
{
private:
    ParseManager* parser;

public:
    explicit TransmitParser(ParseManager* parser) : parser(parser) { }

    [[nodiscard]] std::shared_ptr<TransmitAstNode> Parse() const;
};
