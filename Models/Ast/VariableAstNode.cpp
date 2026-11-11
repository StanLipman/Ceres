#include "VariableAstNode.h"
#include "Passes/AstVisitor.h"

void VariableAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
