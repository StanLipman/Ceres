
#include "ExpressionAstNode.h"

#include "Passes/AstVisitor.h"

void ExpressionAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
