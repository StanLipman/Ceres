#pragma once
#include "BaseParser.h"
#include "Models/Ast/ConstellationAstNode.h"

class ConstellationParser: public BaseParser<ConstellationAstNode>
{
public:
    explicit ConstellationParser(ParseManager* parser)
        : BaseParser(parser) { }

    void Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<ConstellationAstNode>& astNode) override;
};
