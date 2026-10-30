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

    void Print(const int depth) override
    {
        for (const auto& galaxyAstNode : galaxyAstNodes)
        {
            for (size_t i = 0; i < depth; i++)
                printf("-");
            galaxyAstNode->Print(depth + 1);
        }
    }
};
