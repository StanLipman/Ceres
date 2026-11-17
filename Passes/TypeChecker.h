#pragma once
#include "AstVisitor.h"
#include "Models/CompilerContext.h"
#include "Models/Ast/DocumentAstNode.h"
#include "Models/Ast/GalaxyAstNode.h"
#include "Models/Ast/ArgumentAstNode.h"
#include "Models/Ast/BodyAstNode.h"
#include "Models/Ast/ConstellationAstNode.h"
#include "Models/Ast/LiteralValueAstNode.h"
#include "Models/Ast/MethodAstNode.h"
#include "Models/Ast/MethodCallAstNode.h"
#include "Models/Ast/TransmitAstNode.h"
#include "Models/Ast/TypeAstNode.h"
#include "Models/Ast/VariableAstNode.h"
#include "Models/Symbols/ConstellationSymbol.h"
#include "Models/Symbols/MethodSymbol.h"
#include "TypeChecks/TransmitStatementTypeChecker.h"
#include "TypeChecks/MethodCallStatementTypeChecker.h"

class TypeChecker : public AstVisitor
{
private:
    CompilerContext* compilerContext;

public:
    explicit TypeChecker(CompilerContext* compiler_context) : compilerContext(compiler_context) { }

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

        TransmitStatementTypeChecker::ValidateStatement(compilerContext, node);
        MethodCallStatementTypeChecker::ValidateStatement(compilerContext, node);
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

    void visit(VariableAstNode& node) override
    {
        // Skip? Handled in MethodAstNode
    }

    void visit(MethodCallAstNode& node) override
    {
        // TODO: Do we need to handle this?
        // Maybe we here do the validation if the method exists, and if the passed arguments are valid
    }

    void visit(MemberAccessAstNode& node) override
    {
        // TODO: Maybe
    }

    static std::shared_ptr<Type> GetType(const std::shared_ptr<Scope>& scope, const std::shared_ptr<AstNode>& node)
    {
        // This is dirty, and only works for transmit statements. Otherwise this is broken logic
        if (node == nullptr)
        {
            const auto& symbol = scope->Lookup(GetTokenKindText(TokenKind::vacuum));
            const auto& constellation = dynamic_cast<ConstellationSymbol*>(symbol.get());
            return constellation->type;
        }

        if (const auto& literalValueAstNode = std::dynamic_pointer_cast<LiteralValueAstNode>(node); literalValueAstNode != nullptr)
            return GetType(scope, literalValueAstNode);

        if (const auto& variableAstNode = std::dynamic_pointer_cast<VariableAstNode>(node); variableAstNode != nullptr)
            return GetType(scope, variableAstNode);

        assert(false); // What other ways of transmitting do we have?
        return nullptr;
    }

    static std::shared_ptr<Type> GetType(const std::shared_ptr<Scope>& scope, const std::shared_ptr<LiteralValueAstNode>& node)
    {
        const auto& symbol = scope->Lookup(GetTokenKindText(node->tokenKind));
        const auto& constellation = dynamic_cast<ConstellationSymbol*>(symbol.get());
        return constellation->type;
    }

    static std::shared_ptr<Type> GetType(const std::shared_ptr<Scope>& scope, const std::shared_ptr<VariableAstNode>& node)
    {
        const auto& symbol = scope->Lookup(node->name);
        if (symbol == nullptr)
            return nullptr;

        const auto& variableSymbol = std::dynamic_pointer_cast<VariableSymbol>(symbol);
        if (variableSymbol != nullptr)
            return variableSymbol->type;

        const auto& constellationSymbol = std::dynamic_pointer_cast<ConstellationSymbol>(symbol);
        assert(constellationSymbol != nullptr);
        return constellationSymbol->type;
    }
};
