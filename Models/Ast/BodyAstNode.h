#pragma once
#include <memory>
#include <vector>

#include "AstNodes.h"

class BodyAstNode : public AstNode
{
private:
    std::vector<std::shared_ptr<AstNode>> statementAstNodes;

public:
    void AddStatement(const std::shared_ptr<AstNode>& statementAstNode)
    {
        statementAstNodes.push_back(statementAstNode);
    }

    void Print(const int depth) override
    {
        for (const auto& statementAstNode : statementAstNodes)
        {
            statementAstNode->Print(depth);
        }
    }
};
