#pragma once
#include <memory>

#include "AstVisitor.h"
#include "Models/Ast/DocumentAstNode.h"
#include "Models/Scopes/Scope.h"
#include "Models/Symbols/ConstellationSymbol.h"
#include "Models/Symbols/MethodSymbol.h"

class DeclarationCollector : AstVisitor
{
private:
    std::shared_ptr<Scope> currentScope;
    CompilerContext compilerContext;

public:
    explicit DeclarationCollector(const std::shared_ptr<Scope>& globalScope, const CompilerContext& compilerContext) : currentScope(globalScope), compilerContext(compilerContext) {}

    void visit(DocumentAstNode& node) override
    {
        const auto documentScope = std::make_shared<Scope>(Scope::ScopeTypes::document, currentScope);
        currentScope = documentScope;
        node.SetScope(documentScope);

        for (const auto& galaxyAstNode : node.GetGalaxyAstNodes())
            galaxyAstNode->accept(*this);

        currentScope = documentScope->parent;
    }

    void visit(GalaxyAstNode& node) override
    {
        const auto galaxyScope = std::make_shared<Scope>(Scope::ScopeTypes::galaxy, currentScope);
        currentScope = galaxyScope;
        node.SetScope(galaxyScope);

        node.GetBody()->accept(*this);

        currentScope = currentScope->parent;
    }

    void visit(ArgumentAstNode& node) override
    {
        // Skip?
    }

    void visit(BodyAstNode& node) override
    {
        // Don't create scopes here, as this is always part of a parent which handles creating scopes itself
        for (const auto& statement : node.GetStatements())
            statement->accept(*this);
    }

    void visit(ConstellationAstNode& node) override
    {
        const auto symbol = std::make_shared<ConstellationSymbol>();
        symbol->symbolId = compilerContext.GetNextSymbolId();
        symbol->name = node.GetName();
        symbol->declaration = &node;

        const auto type = std::make_shared<ConstellationType>();
        type->id = compilerContext.GetNextTypeId();
        type->kind = constellation;
        type->declarationSymbolId = symbol->symbolId;

        symbol->type = type;

        if (!currentScope->Insert(symbol))
        {
            printf("Constellation '%s' already exists\n", symbol->name.c_str());
            return;
        }

        const auto constellationScope = std::make_shared<Scope>(Scope::ScopeTypes::constellation, currentScope);
        currentScope = constellationScope;
        node.SetScope(constellationScope);

        node.GetBody()->accept(*this);

        currentScope = currentScope->parent;
    }

    void visit(MethodAstNode& node) override
    {
        const auto symbol = std::make_shared<MethodSymbol>();
        symbol->symbolId = compilerContext.GetNextSymbolId();
        symbol->name = node.GetName();
        symbol->declaration = &node;

        if (!currentScope->Insert(symbol))
        {
            printf("Method '%s' already exists\n", symbol->name.c_str());
            return;
        }

        const auto methodScope = std::make_shared<Scope>(Scope::ScopeTypes::method, currentScope);
        currentScope = methodScope;
        node.SetScope(methodScope);

        node.GetBody()->accept(*this); // TODO: Don't know if this is really needed

        currentScope = currentScope->parent;
    }

    void visit(TypeAstNode& node) override
    {
        // Skip?
    }
};
