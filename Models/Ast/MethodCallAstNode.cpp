#include "MethodCallAstNode.h"
#include "Passes/AstVisitor.h"

void MethodCallAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
