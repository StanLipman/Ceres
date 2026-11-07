#pragma once
#include "BaseParser.h"
#include "Models/Ast/LiteralValueAstNode.h"

class LiteralStringValueParser: public BaseParser<LiteralStringValueAstNode>
{
public:
    explicit LiteralStringValueParser(ParseManager* parser) : BaseParser(parser) { }

    void Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<LiteralStringValueAstNode>& stringValueAstNode) override;
};

class LiteralNumericValueParser: public BaseParser<LiteralNumericValueAstNode>
{
public:
    explicit LiteralNumericValueParser(ParseManager* parser) : BaseParser(parser) { }

    void Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<LiteralNumericValueAstNode>& numericValueAstNode) override;
};
