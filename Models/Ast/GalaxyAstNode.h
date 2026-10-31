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

    std::string GetName()
    {
        return this->identifierName;
    }

    void SetBody(const std::shared_ptr<BodyAstNode>& body)
    {
        assert(this->bodyAstNode == nullptr);
        this->bodyAstNode = body;
    }

    std::shared_ptr<BodyAstNode> GetBody()
    {
        return this->bodyAstNode;
    }

    void accept(AstVisitor& visitor) override;
};
