#pragma once
#include <memory>

#include "AstNodes.h"
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
    std::shared_ptr<AstNode> assignee;
};

class VariableDeclarationAstNode: public ExpressionAstNode
{
public:
    Token typeToken = EmptyToken;
    std::string name;
    std::shared_ptr<AssignExpressionAstNode> assignmentExpression;
};
