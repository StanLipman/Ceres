#include "TypeAstNode.h"
#include "Passes/AstVisitor.h"

void TypeAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
