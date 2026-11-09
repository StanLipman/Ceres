#include "MethodAstNode.h"
#include "Passes/AstVisitor.h"

void MethodAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
