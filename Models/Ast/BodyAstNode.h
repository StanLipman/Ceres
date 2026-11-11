#pragma once
#include <memory>
#include <vector>

#include "AstNodes.h"
#include "Models/Token.h"

class BodyAstNode : public AstNode
{
public:
    std::vector<std::shared_ptr<AstNode>> statementAstNodes;
    Token leftCurlyToken = EmptyToken;
    Token rightCurlyToken = EmptyToken;

    void accept(AstVisitor& visitor) override;
};
