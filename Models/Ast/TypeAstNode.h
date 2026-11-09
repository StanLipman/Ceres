#pragma once
#include <string>

#include "AstNodes.h"

class TypeAstNode : public AstNode
{
public:
    std::string name;

    void accept(AstVisitor& visitor) override;
};
