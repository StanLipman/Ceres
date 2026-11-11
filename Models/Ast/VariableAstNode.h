#pragma once
#include <string>

#include "AstNodes.h"
#include "Models/Token.h"

class VariableAstNode : public AstNode
{
public:
    std::string name;
    Token token = EmptyToken;

    void accept(AstVisitor& visitor) override;
};
