#include "ConstellationAstNode.h"
#include "Passes/AstVisitor.h"

void ConstellationAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
