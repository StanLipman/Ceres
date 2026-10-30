#pragma once
#include "BaseParser.h"
#include "Models/Ast/GalaxyAstNode.h"

class ParseManager;

class GalaxyParser: public BaseParser<GalaxyAstNode>
{
public:
    explicit GalaxyParser(ParseManager* parser)
        : BaseParser(parser) { }

    void Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<GalaxyAstNode>& galaxy) override;
};
