#include "TransmitAstNode.h"
#include "Passes/AstVisitor.h"

void TransmitAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
