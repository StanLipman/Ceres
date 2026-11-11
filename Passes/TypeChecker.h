#pragma once
#include "AstVisitor.h"

class TypeChecker : public AstVisitor
{
public:
    void visit(DocumentAstNode& node) override
    {
        for (const auto& galaxyAstNode : node.galaxyAstNodes)
            galaxyAstNode->accept(*this);
    }

    void visit(GalaxyAstNode& node) override
    {
        node.bodyAstNode->accept(*this);
    }

    void visit(ArgumentAstNode& node) override
    {
        // Skip
    }

    void visit(BodyAstNode& node) override
    {
        for (const auto& statementAstNode : node.statementAstNodes)
            statementAstNode->accept(*this);
    }

    void visit(ConstellationAstNode& node) override
    {
        node.bodyAstNode->accept(*this);
    }

    void visit(MethodAstNode& node) override
    {
        assert(!node.name.empty());
        assert(node.scope != nullptr);
        const auto* methodSymbol = dynamic_cast<MethodSymbol*>(node.scope->Lookup(node.name).get());

        assert(methodSymbol != nullptr);
        assert(methodSymbol->type != nullptr);
        const auto& methodType = methodSymbol->type;
        assert(methodType->returnType != nullptr);
        const auto& returnType = methodType->returnType;
        assert(returnType != nullptr);

        for (const auto& statementAstNode : node.bodyAstNode->statementAstNodes)
        {
            // Right now a body can only contain transmit statements
            const auto* transmitStatement = dynamic_cast<TransmitAstNode*>(statementAstNode.get());
            assert(transmitStatement != nullptr);
            const auto& transmitValue = transmitStatement->transmitValue;

            const std::shared_ptr<Type>& transmitValueType = GetType(node.scope, transmitValue);
            if (transmitValueType != returnType)
            {
                printf("Method '%s' expected transmit type of '%s', but '%s' got transmitted.\n", node.name.c_str(), node.returnTypeAstNode->name.c_str(), GetTokenKindText(transmitStatement->transmitValue->tokenKind).c_str());
                return;
            }
        }
    }

    void visit(TypeAstNode& node) override
    {
        // Skip?
    }

    void visit(TransmitAstNode& node) override
    {
        // Skip? Handled in MethodAstNode?
    }

    void visit(LiteralValueAstNode& node) override
    {
        // Skip? Handled in MethodAstNode
    }

private:
    static std::shared_ptr<Type> GetType(const std::shared_ptr<Scope>& scope, const std::shared_ptr<LiteralValueAstNode>& node)
    {
        // This is dirty, and only works for transmit statements. Otherwise this is broken logic
        if (node == nullptr)
        {
            const auto& symbol = scope->Lookup(GetTokenKindText(TokenKind::vacuum));
            const auto& constellation = dynamic_cast<ConstellationSymbol*>(symbol.get());
            return constellation->type;
        }

        const auto& symbol = scope->Lookup(GetTokenKindText(node->tokenKind));
        const auto& constellation = dynamic_cast<ConstellationSymbol*>(symbol.get());
        return constellation->type;
    }
};