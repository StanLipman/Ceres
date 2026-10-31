#pragma once
#include <memory>
#include <vector>

#include "AstNodes.h"
#include "GalaxyAstNode.h"

class DocumentAstNode : public AstNode
{
private:
    std::vector<std::shared_ptr<GalaxyAstNode>> galaxyAstNodes;

public:
    void AddGalaxyAstNode(const std::shared_ptr<GalaxyAstNode>& galaxyAstNode)
    {
        galaxyAstNodes.push_back(galaxyAstNode);
    }

    std::vector<std::shared_ptr<GalaxyAstNode>>& GetGalaxyAstNodes()
    {
        return galaxyAstNodes;
    }

    void accept(AstVisitor& visitor) override;
};
