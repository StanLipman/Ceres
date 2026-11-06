#pragma once

class AstVisitor;

class AstNode
{
public:
    virtual ~AstNode() = default;
    virtual void accept(AstVisitor& visitor) = 0;
};
