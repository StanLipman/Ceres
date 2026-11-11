#pragma once
#include "AstNodes.h"
#include "LiteralValueAstNode.h"
#include <memory>

class TransmitAstNode :  public AstNode
{
public:
    std::shared_ptr<AstNode> transmitValue;
    Token token = EmptyToken;

    void accept(AstVisitor& visitor) override;
};
