#pragma once
#include <memory>

#include "AstVisitor.h"
#include "Models/Ast/DocumentAstNode.h"
#include "Models/Scopes/Scope.h"
#include "Models/Symbols/MethodSymbol.h"

class DeclarationResolver: AstVisitor
{
private:
    std::shared_ptr<Scope> currentScope;

public:
    explicit DeclarationResolver(const std::shared_ptr<Scope>& globalScope) : currentScope(globalScope) {}

    void visit(DocumentAstNode& node) override
    {
        currentScope = node.GetScope();
        for (const auto& galaxyAstNode : node.GetGalaxyAstNodes())
            galaxyAstNode->accept(*this);

        currentScope = currentScope->parent;
    }

    void visit(GalaxyAstNode& node) override
    {
        currentScope = node.GetScope();

        node.GetBody()->accept(*this);

        currentScope = currentScope->parent;
    }

    void visit(BodyAstNode& node) override
    {
        // No scope switching, as it is always part of a parent, which handles the switching

        for (const auto& statement : node.GetStatements())
            statement->accept(*this);
    }

    void visit(ConstellationAstNode& node) override
    {
        currentScope = node.GetScope();

        node.GetBody()->accept(*this);

        currentScope = currentScope->parent;
    }

    void visit(MethodAstNode& node) override
    {
        auto* symbol = dynamic_cast<MethodSymbol*>(currentScope->Lookup(node.GetName()).get());

        if (symbol == nullptr)
        {
            printf("Missing method symbol\n");
            return;
        }

        const auto methodType = std::make_shared<MethodType>();
        methodType->kind = method;
        methodType->returnType = ResolveType(*node.GetReturnType());

        for (const auto& argumentAstNode : node.GetArguments())
        {
            auto parameterType = ResolveType(*argumentAstNode->GetType());

            methodType->parameterTypes.push_back(parameterType);
        }

        symbol->type = methodType;

        currentScope = node.GetScope();

        for (size_t i = 0; i < methodType->parameterTypes.size(); ++i)
        {
            const auto& argumentAstNode = node.GetArguments()[i];

            const auto variableSymbol = std::make_shared<VariableSymbol>();
            variableSymbol->name = argumentAstNode->GetName();
            variableSymbol->type = symbol->type->parameterTypes[i];

            if (!currentScope->Insert(variableSymbol))
            {
                printf("Duplicate parameter '%s'\n", argumentAstNode->GetName().c_str());
                continue;
            }
        }

        node.GetBody()->accept(*this);

        currentScope = currentScope->parent;
    }

    void visit(ArgumentAstNode& node) override
    {
        // We don't process arguments here, because we do that in the method resolver.
        // We do it there, because we need some information from the method itself
    }

    void visit(TypeAstNode& node) override
    {
        // Skip?
    }

private:
    std::shared_ptr<Type> ResolveType(TypeAstNode& typeAstNode) const
    {
        const std::shared_ptr<Symbol> symbol = currentScope->Lookup(typeAstNode.GetName());

        if (symbol == nullptr)
        {
            printf("Unknown type: %s\n", typeAstNode.GetName().c_str());
            return nullptr;;
        }

        const auto* constellationSymbol = dynamic_cast<ConstellationSymbol*>(symbol.get());

        if (constellationSymbol == nullptr)
        {
            printf("%s is not a type\n", typeAstNode.GetName().c_str());
            return nullptr;
        }

        return constellationSymbol->type;
    }
};
