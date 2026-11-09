#pragma once
#include <memory>
#include <vector>

#include "AstNodes.h"
#include "GalaxyAstNode.h"
#include "Models/Scopes/Scope.h"

class DocumentAstNode : public AstNode
{
public:
    std::vector<std::shared_ptr<GalaxyAstNode>> galaxyAstNodes;
    std::shared_ptr<Scope> scope;

    void accept(AstVisitor& visitor) override;
};
