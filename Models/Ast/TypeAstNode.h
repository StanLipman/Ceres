#pragma once
#include "AstNodes.h"

class TypeAstNode : public AstNode
{
private:
    std::string identifierName;

public:
    void SetName(const std::string& galaxyName)
    {
        this->identifierName = galaxyName;
    }

    void Print(int depth) override
    {
        printf("%s", identifierName.c_str());
    }
};