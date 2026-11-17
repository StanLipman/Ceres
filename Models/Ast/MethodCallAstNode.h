#pragma once
#include <memory>
#include <vector>

#include "AstNodes.h"
#include "MemberAccessAstNode.h"
#include "Models/Token.h"

class MethodCallAstNode : public AstNode
{
public:
    std::vector<std::shared_ptr<AstNode>> arguments;
    std::shared_ptr<MemberAccessAstNode> callee;
    Token token;

    void accept(AstVisitor& visitor) override;
};
