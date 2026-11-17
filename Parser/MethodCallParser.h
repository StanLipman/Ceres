#pragma once
#include "Models/Ast/MethodCallAstNode.h"

class ParseManager;

class MethodCallParser
{
private:
    ParseManager* parser;

public:
    explicit MethodCallParser(ParseManager* parser) : parser(parser) {  }

    [[nodiscard]] std::shared_ptr<MethodCallAstNode> Parse() const;
};

