#pragma once
#include "AstNodes.h"
#include "Models/Token.h"

class AutoAstNode: public AstNode
{
public:
    Token token = EmptyToken;

    void accept(AstVisitor& visitor) override;
};
