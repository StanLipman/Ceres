#pragma once
#include <memory>

#include "Models/Ast/AstNodes.h"

class ParseManager;

template <typename T>
class BaseParser
{
protected:
    ParseManager* parser;

public:
    explicit BaseParser(ParseManager* parser)
    {
        this->parser = parser;
    }

    virtual ~BaseParser() = default;
    virtual void Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<T>& astNode) = 0;
};
