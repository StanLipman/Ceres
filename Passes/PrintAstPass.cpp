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

    for (const auto& galaxyAstNode : node.galaxyAstNodes)
    {
        galaxyAstNode->accept(*this);
    }
    currentDepth--;
}

void PrintAstPass::visit(GalaxyAstNode& node)
{
    for (size_t i = 0; i < currentDepth; i++)
        printf("-");

    assert(!node.name.empty());
    printf("galaxy %s\n", node.name.c_str());

    const auto body = node.bodyAstNode;
    if (body == nullptr)
        return;

    body->accept(*this);
}

void PrintAstPass::visit(ArgumentAstNode& node)
{
    assert(node.typeAstNode != nullptr);
    const auto type = node.typeAstNode;
    type->accept(*this);
    assert(!node.name.empty());
    printf(" %s", node.name.c_str());
}

void PrintAstPass::visit(BodyAstNode& node)
{
    currentDepth++;
    for (const auto& statement : node.statementAstNodes)
    {
        statement->accept(*this);
    }
    currentDepth--;
}

void PrintAstPass::visit(ConstellationAstNode& node)
{
    for (size_t i = 0; i < currentDepth; i++)
        printf("-");

    assert(node.accessModifier != TokenKind::none);
    assert(!node.name.empty());
    assert(node.bodyAstNode != nullptr);
    const auto accessModifier = node.accessModifier;
    const auto name = node.name;
    const auto body = node.bodyAstNode;

    printf("%s constellation %s\n", GetTokenKindText(accessModifier).c_str(), name.c_str());

    if (body == nullptr)
        return;

    body->accept(*this);
}

void PrintAstPass::visit(MethodAstNode& node)
{
    for (size_t i = 0; i < currentDepth; i++)
        printf("-");

    assert(node.accessModifier != TokenKind::none);
    printf("%s ", GetTokenKindText(node.accessModifier).c_str());

    assert(node.returnTypeAstNode != nullptr);
    node.returnTypeAstNode->accept(*this);

    assert(!node.name.empty());
    printf(" %s(", node.name.c_str());

    const auto arguments = node.argumentAstNodes;
    for (size_t i = 0; i < arguments.size(); i++)
    {
        arguments.at(i)->accept(*this);
        if (i < arguments.size() - 1)
            printf(", ");
    }

    printf(")");

    assert(node.bodyAstNode != nullptr);
    node.bodyAstNode->accept(*this);
    printf("\n");
}

void PrintAstPass::visit(TypeAstNode& node)
{
    assert(!node.name.empty());
    printf("%s", node.name.c_str());
}
