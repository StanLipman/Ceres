#pragma once
#include "BaseParser.h"
#include "Models/Ast/TransmitAstNode.h"

class TransmitParser: public BaseParser<TransmitAstNode>
{
public:
    explicit TransmitParser(ParseManager* parser) : BaseParser(parser) { }

    void Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<TransmitAstNode>& transmitAstNode) override;
};
