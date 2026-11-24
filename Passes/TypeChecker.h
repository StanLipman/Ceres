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
#include "Models/Ast/TypeAstNode.h"
#include "Models/Ast/VariableAstNode.h"
#include "Models/Ast/ExpressionAstNode.h"
#include "Models/Symbols/ConstellationSymbol.h"
#include "Models/Symbols/MethodSymbol.h"
#include "SemanticChecks/MethodHasTransmitStatementChecker.h"
#include "TypeChecks/ExpressionStatementTypeChecker.h"
#include "TypeChecks/TransmitStatementTypeChecker.h"
#include "TypeChecks/MethodCallStatementTypeChecker.h"

class TypeChecker : public AstVisitor
{
private:
    CompilerContext* compilerContext;
    std::shared_ptr<Scope> currentScope;

public:
    explicit TypeChecker(CompilerContext* compiler_context) : compilerContext(compiler_context) { }

    void visit(DocumentAstNode& node) override
    {
        assert(node.scope != nullptr);
        currentScope = node.scope;

        for (const auto& galaxyAstNode : node.galaxyAstNodes)
            galaxyAstNode->accept(*this);

        assert(currentScope->parent != nullptr);
        currentScope = currentScope->parent;
    }

    void visit(GalaxyAstNode& node) override
    {
        assert(node.scope != nullptr);
        currentScope = node.scope;

        node.bodyAstNode->accept(*this);

        assert(currentScope->parent != nullptr);
        currentScope = currentScope->parent;
    }

    void visit(ArgumentAstNode& node) override
    {
        node.typeAstNode->accept(*this);
    }

    void visit(BodyAstNode& node) override
    {
        for (const auto& statementAstNode : node.statementAstNodes)
            statementAstNode->accept(*this);
    }

    void visit(ConstellationAstNode& node) override
    {
        assert(node.scope != nullptr);
        currentScope = node.scope;

        node.bodyAstNode->accept(*this);

        assert(currentScope->parent != nullptr);
        currentScope = currentScope->parent;
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

        assert(node.scope != nullptr);
        currentScope = node.scope;

        MethodHasTransmitStatementChecker::ValidateMethod(compilerContext, node);

        for (const auto& argumentAstNode : node.argumentAstNodes)
            argumentAstNode->accept(*this);

        node.returnTypeAstNode->accept(*this);
        node.bodyAstNode->accept(*this);

        assert(currentScope->parent != nullptr);
        currentScope = currentScope->parent;
    }

    void visit(TypeAstNode& node) override
    {
        // Skip
    }

    void visit(TransmitAstNode& node) override
    {
        if (node.transmitValue != nullptr)
            node.transmitValue->accept(*this);

        TransmitStatementTypeChecker::ValidateTransmitStatement(compilerContext, currentScope, node);
    }

    void visit(LiteralValueAstNode& node) override
    {
        // Skip
    }

    void visit(VariableAstNode& node) override
    {
        // Skip
    }

    void visit(MethodCallAstNode& node) override
    {
        for (const auto& argument : node.arguments)
            argument->accept(*this);

        node.callee->accept(*this);

        MethodCallStatementTypeChecker::ValidateMethodCall(compilerContext, currentScope, node);
    }

    void visit(MemberAccessAstNode& node) override
    {
        if (node.object == nullptr)
            return;

        node.object->accept(*this);
    }

    void visit(ExpressionAstNode& node) override
    {
        const auto* variableDeclarationAstNode = dynamic_cast<VariableDeclarationAstNode*>(&node);
        if (variableDeclarationAstNode != nullptr)
        {
            variableDeclarationAstNode->type->accept(*this);
            variableDeclarationAstNode->assignmentExpression->accept(*this);

            ExpressionStatementTypeChecker::ValidateVariableDeclarationStatement(compilerContext, currentScope, *variableDeclarationAstNode);
        }
    }

    void visit(AutoAstNode& node) override
    {
        // Skip
    }

    static std::shared_ptr<Type> GetType(const std::shared_ptr<Scope>& scope, const AstNode* node)
    {
        // This is dirty, and only works for transmit statements. Otherwise this is broken logic
        if (node == nullptr)
        {
            const auto& symbol = scope->Lookup(GetTokenKindText(TokenKind::vacuum));
            const auto& constellation = dynamic_cast<ConstellationSymbol*>(symbol.get());
            return constellation->type;
        }

        if (const auto* literalValueAstNode = dynamic_cast<const LiteralValueAstNode*>(node); literalValueAstNode != nullptr)
            return GetType(scope, literalValueAstNode);

        if (const auto* variableAstNode = dynamic_cast<const VariableAstNode*>(node); variableAstNode != nullptr)
            return GetType(scope, variableAstNode);

        if (const auto* typeAstNode = dynamic_cast<const TypeAstNode*>(node); typeAstNode != nullptr)
        {
            const auto& symbol = scope->Lookup(typeAstNode->name);
            const auto& constellation = std::dynamic_pointer_cast<ConstellationSymbol>(symbol);
            assert(constellation != nullptr);
            return constellation->type;
        }

        assert(false); // What other ways of transmitting do we have?
        return nullptr;
    }

    static std::shared_ptr<Type> GetType(const std::shared_ptr<Scope>& scope, const LiteralValueAstNode* node)
    {
        const auto& symbol = scope->Lookup(GetTokenKindText(node->tokenKind));
        const auto& constellation = dynamic_cast<ConstellationSymbol*>(symbol.get());
        return constellation->type;
    }

    static std::shared_ptr<Type> GetType(const std::shared_ptr<Scope>& scope, const VariableAstNode* node)
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
