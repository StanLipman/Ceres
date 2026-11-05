#include "PrintAstPass.h"
#include "Models/Ast/DocumentAstNode.h"
#include "Models/Ast/GalaxyAstNode.h"
#include "Models/Ast/ArgumentAstNode.h"
#include "Models/Ast/BodyAstNode.h"
#include "Models/Ast/ConstellationAstNode.h"
#include "Models/Ast/MethodAstNode.h"
#include "Models/Ast/TypeAstNode.h"

void PrintAstPass::visit(DocumentAstNode& node) {
    currentDepth++;
    for (const auto& galaxyAstNode : node.GetGalaxyAstNodes())
    {
        galaxyAstNode->accept(*this);
    }
    currentDepth--;
}

void PrintAstPass::visit(GalaxyAstNode& node)
{
    for (size_t i = 0; i < currentDepth; i++)
        printf("-");

    printf("galaxy %s\n", node.GetName().c_str());

    const auto body = node.GetBody();
    if (body == nullptr)
        return;

    body->accept(*this);
}

void PrintAstPass::visit(ArgumentAstNode& node)
{
    const auto type = node.GetType();
    type->accept(*this);
    printf(" %s", node.GetName().c_str());
}

void PrintAstPass::visit(BodyAstNode& node)
{
    currentDepth++;
    for (const auto& statement : node.GetStatements())
    {
        statement->accept(*this);
    }
    currentDepth--;
}

void PrintAstPass::visit(ConstellationAstNode& node)
{
    for (size_t i = 0; i < currentDepth; i++)
        printf("-");

    const auto accessModifier = node.GetAccessModifier();
    const auto name = node.GetName();
    const auto body = node.GetBody();

    printf("%s constellation %s\n", GetTokenKindText(accessModifier).c_str(), name.c_str());

    if (body == nullptr)
        return;

    body->accept(*this);
}

void PrintAstPass::visit(MethodAstNode& node)
{
    for (size_t i = 0; i < currentDepth; i++)
        printf("-");

    printf("%s ", GetTokenKindText(node.GetAccessModifier()).c_str());
    node.GetReturnType()->accept(*this);
    printf(" %s(", node.GetName().c_str());

    const auto arguments = node.GetArguments();
    for (size_t i = 0; i < arguments.size(); i++)
    {
        arguments.at(i)->accept(*this);
        if (i < arguments.size() - 1)
            printf(", ");
    }

    printf(")");
    node.GetBody()->accept(*this);
    printf("\n");
}

void PrintAstPass::visit(TypeAstNode& node)
{
    printf("%s", node.GetName().c_str());
}
