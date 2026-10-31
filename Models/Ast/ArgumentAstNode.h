#pragma once
#include <memory>
#include <string>

#include "AstNodes.h"
#include "TypeAstNode.h"

class ArgumentAstNode : public AstNode
{
private:
    std::string argumentIdentifierName;
    std::shared_ptr<TypeAstNode> argumentTypeAstNode;

public:
    void SetName(const std::string& argumentName)
    {
        argumentIdentifierName = argumentName;
    }

    std::string GetName()
    {
        return argumentIdentifierName;
    }

    void SetType(const std::shared_ptr<TypeAstNode>& argumentType)
    {
        argumentTypeAstNode = argumentType;
    }

    std::shared_ptr<TypeAstNode> GetType()
    {
        return argumentTypeAstNode;
    }

    void accept(AstVisitor& visitor) override;
};
