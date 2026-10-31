#include "GalaxyAstNode.h"
#include "Passes/AstVisitor.h"

void GalaxyAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
