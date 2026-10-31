#include "BodyAstNode.h"
#include "Passes/AstVisitor.h"

void BodyAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
