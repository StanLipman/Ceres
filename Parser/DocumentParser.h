#pragma once
#include "Models/Ast/DocumentAstNode.h"

class ParseManager;

class DocumentParser
{
private:
    ParseManager* parser;

public:
    explicit DocumentParser(ParseManager* parser) : parser(parser) { }

    [[nodiscard]] std::shared_ptr<DocumentAstNode> Parse() const;
};
