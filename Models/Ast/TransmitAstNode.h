#pragma once
#include "AstNodes.h"
#include "LiteralValueAstNode.h"
#include <memory>

class TransmitAstNode :  public AstNode
{
public:
    std::shared_ptr<LiteralValueAstNode> transmitValue;
    Token token = EmptyToken;

    void accept(AstVisitor& visitor) override;
};
