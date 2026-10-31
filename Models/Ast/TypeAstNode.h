#pragma once
#include <cassert>

#include "AstNodes.h"

class TypeAstNode : public AstNode
{
private:
    std::string typeIdentifierName;

public:
    void SetName(const std::string& typeName)
    {
        assert(typeIdentifierName.empty());
        typeIdentifierName = typeName;
    }

    void Print(int depth) override
    {
        printf("%s", typeIdentifierName.c_str());
    }
};
