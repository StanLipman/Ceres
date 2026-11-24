#pragma once
#include "AstNodes.h"
#include "Models/Token.h"

class AutoAstNode: public AstNode
{
public:
    Token token;

    void accept(AstVisitor& visitor) override;
};
