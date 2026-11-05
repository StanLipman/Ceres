#pragma once
#include <memory>
#include <vector>

#include "AstNodes.h"
#include "GalaxyAstNode.h"
#include "Models/Scopes/Scope.h"

class DocumentAstNode : public AstNode
{
private:
    std::vector<std::shared_ptr<GalaxyAstNode>> galaxyAstNodes;
    std::shared_ptr<Scope> scope;

public:
    void AddGalaxyAstNode(const std::shared_ptr<GalaxyAstNode>& galaxyAstNode)
    {
        galaxyAstNodes.push_back(galaxyAstNode);
    }

    std::vector<std::shared_ptr<GalaxyAstNode>>& GetGalaxyAstNodes()
    {
        return galaxyAstNodes;
    }

    void SetScope(const std::shared_ptr<Scope>& scopeToSet)
    {
        assert(scope == nullptr);
        scope = scopeToSet;
    }

    std::shared_ptr<Scope> GetScope()
    {
        assert(scope != nullptr);
        return scope;
    }

    void accept(AstVisitor& visitor) override;
};
