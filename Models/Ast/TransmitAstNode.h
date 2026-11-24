#pragma once
#include "AstNodes.h"
#include <memory>

#include "MethodAstNode.h"
#include "Models/Token.h"

class TransmitAstNode :  public AstNode
{
public:
    std::shared_ptr<AstNode> transmitValue;
    std::shared_ptr<MethodAstNode> parent;
    Token token = EmptyToken;

    void accept(AstVisitor& visitor) override;
};
