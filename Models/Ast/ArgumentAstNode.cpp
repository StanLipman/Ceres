#include "ArgumentAstNode.h"
#include "Passes/AstVisitor.h"

void ArgumentAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
