#pragma once

class AstNode
{
public:
    virtual ~AstNode() = default;
    virtual void Print(int depth) = 0;
};






