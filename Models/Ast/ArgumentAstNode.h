#pragma once
#include <memory>
#include <string>

#include "AstNodes.h"
#include "TypeAstNode.h"

class ArgumentAstNode : public AstNode
{
public:
    std::string name;
    std::shared_ptr<TypeAstNode> typeAstNode;

    void accept(AstVisitor& visitor) override;
};
