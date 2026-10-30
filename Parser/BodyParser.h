#pragma once
#include "BaseParser.h"
#include "Models/Ast/BodyAstNode.h"

class BodyParser: public BaseParser<BodyAstNode>
{
public:
    explicit BodyParser(ParseManager* parser)
        : BaseParser(parser) { }

    void Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<BodyAstNode>& astNode) override;
};
