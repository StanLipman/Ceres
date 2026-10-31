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

    void SetType(const std::shared_ptr<TypeAstNode>& argumentType)
    {
        argumentTypeAstNode = argumentType;
    }

    void Print(const int depth) override
    {
        assert(argumentTypeAstNode != nullptr);

        argumentTypeAstNode->Print(depth);
        printf(" %s", argumentIdentifierName.c_str());
    }
};
