#pragma once
#include "BaseParser.h"
#include "Models/Ast/MethodAstNode.h"

class MethodParser: BaseParser<MethodAstNode>
{
public:
    explicit MethodParser(ParseManager* parser)
        : BaseParser(parser) { }

    void Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<MethodAstNode>& astNode) override;
    void ParseArgument(const std::shared_ptr<MethodAstNode>& method);
};
