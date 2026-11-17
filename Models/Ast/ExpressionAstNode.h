#pragma once
#include <memory>

#include "AstNodes.h"
#include "TypeAstNode.h"
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
    std::string name;
    std::shared_ptr<TypeAstNode> type;
    std::shared_ptr<AssignExpressionAstNode> assignmentExpression;
};
