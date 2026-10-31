#pragma once
#include <cassert>
#include <memory>

#include "AstNodes.h"
#include "BodyAstNode.h"

class GalaxyAstNode : public AstNode
{
private:
    std::string identifierName;
    std::shared_ptr<BodyAstNode> bodyAstNode;

public:
    void SetName(const std::string& galaxyName)
    {
        this->identifierName = galaxyName;
    }

    void SetBody(const std::shared_ptr<BodyAstNode>& body)
    {
        assert(this->bodyAstNode == nullptr);
        this->bodyAstNode = body;
    }

    void Print(const int depth) override
    {
        for (size_t i = 0; i < depth; i++)
            printf("-");
        printf("galaxy %s\n", this->identifierName.c_str());

        if (bodyAstNode == nullptr)
            return;

        bodyAstNode->Print(depth + 1);
    }
};
