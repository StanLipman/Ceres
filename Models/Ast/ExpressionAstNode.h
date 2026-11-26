#pragma once
#include <memory>

#include "AstNodes.h"
#include "TypeAstNode.h"
#include "VariableAstNode.h"
#include "Models/Token.h"

class ExpressionAstNode: public AstNode
{
public:
    Token token = EmptyToken;

    void accept(AstVisitor& visitor) override;
};

class AssignExpressionAstNode : public ExpressionAstNode
{
public:
    std::shared_ptr<VariableAstNode> target;
    std::shared_ptr<AstNode> value;
};

class VariableDeclarationAstNode: public ExpressionAstNode
{
public:
    std::string name;
    std::shared_ptr<AstNode> type;
    std::shared_ptr<AstNode> initializer;
};
