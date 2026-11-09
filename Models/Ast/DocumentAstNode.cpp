#include "DocumentAstNode.h"
#include "Passes/AstVisitor.h"

void DocumentAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
