#include "LiteralValueAstNode.h"
#include "Passes/AstVisitor.h"

void LiteralValueAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
