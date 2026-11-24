#include "AutoAstNode.h"

#include "Passes/AstVisitor.h"

void AutoAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
