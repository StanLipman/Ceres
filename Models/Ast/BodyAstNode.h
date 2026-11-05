#pragma once
#include <memory>
#include <vector>

#include "AstNodes.h"

class BodyAstNode : public AstNode
{
public:
    std::vector<std::shared_ptr<AstNode>> statementAstNodes;

    void accept(AstVisitor& visitor) override;
};
