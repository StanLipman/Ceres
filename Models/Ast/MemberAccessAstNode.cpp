#include "MemberAccessAstNode.h"

#include "Passes/AstVisitor.h"

void MemberAccessAstNode::accept(AstVisitor& visitor)
{
    visitor.visit(*this);
}
