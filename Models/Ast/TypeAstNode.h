#pragma once
#include <cassert>
#include <string>

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

    std::string GetName()
    {
        return typeIdentifierName;
    }

    void accept(AstVisitor& visitor) override;
};
