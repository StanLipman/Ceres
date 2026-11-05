#pragma once
#include <cassert>
#include <memory>

#include "AstNodes.h"
#include "BodyAstNode.h"
#include "Models/Scopes/Scope.h"

class GalaxyAstNode : public AstNode
{
public:
    std::string name;
    std::shared_ptr<BodyAstNode> bodyAstNode;
    std::shared_ptr<Scope> scope;

    void accept(AstVisitor& visitor) override;
};
