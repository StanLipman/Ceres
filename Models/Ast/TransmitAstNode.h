#pragma once
#include "AstNodes.h"

class TransmitAstNode :  public AstNode
{
public:
    void accept(AstVisitor& visitor) override;
};
