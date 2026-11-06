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
        assert(node.scope != nullptr);
        currentScope = node.scope;
        for (const auto& galaxyAstNode : node.galaxyAstNodes)
            galaxyAstNode->accept(*this);

        currentScope = currentScope->parent;
    }

    void visit(GalaxyAstNode& node) override
    {
        assert(node.scope != nullptr);
        currentScope = node.scope;

        assert(node.bodyAstNode != nullptr);
        node.bodyAstNode->accept(*this);

        currentScope = currentScope->parent;
    }

    void visit(BodyAstNode& node) override
    {
        // No scope switching, as it is always part of a parent, which handles the switching

        for (const auto& statement : node.statementAstNodes)
            statement->accept(*this);
    }

    void visit(ConstellationAstNode& node) override
    {
        assert(node.scope != nullptr);
        currentScope = node.scope;

        assert(node.bodyAstNode != nullptr);
        node.bodyAstNode->accept(*this);

        currentScope = currentScope->parent;
    }

    void visit(MethodAstNode& node) override
    {
        assert(!node.name.empty());
        auto* symbol = dynamic_cast<MethodSymbol*>(currentScope->Lookup(node.name).get());

        if (symbol == nullptr)
        {
            printf("Missing method symbol\n");
            return;
        }

        const auto methodType = std::make_shared<MethodType>();
        methodType->kind = Type::TypeKind::method;
        assert(node.returnTypeAstNode != nullptr);
        methodType->returnType = ResolveType(*node.returnTypeAstNode);

        for (const auto& argumentAstNode : node.argumentAstNodes)
        {
            assert(argumentAstNode->typeAstNode != nullptr);
            auto parameterType = ResolveType(*argumentAstNode->typeAstNode);

            methodType->parameterTypes.push_back(parameterType);
        }

        symbol->type = methodType;

        assert(node.scope != nullptr);
        currentScope = node.scope;

        assert(methodType->parameterTypes.size() == node.argumentAstNodes.size());
        for (size_t i = 0; i < methodType->parameterTypes.size(); ++i)
        {
            const auto& argumentAstNode = node.argumentAstNodes[i];

            const auto variableSymbol = std::make_shared<VariableSymbol>();
            assert(!argumentAstNode->name.empty());
            variableSymbol->name = argumentAstNode->name;
            variableSymbol->type = symbol->type->parameterTypes[i];

            if (!currentScope->Insert(variableSymbol))
            {
                printf("Duplicate parameter '%s'\n", argumentAstNode->name.c_str());
                continue;
            }
        }

        assert(node.bodyAstNode != nullptr);
        node.bodyAstNode->accept(*this);

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
        assert(!typeAstNode.name.empty());
        const std::shared_ptr<Symbol> symbol = currentScope->Lookup(typeAstNode.name);

        if (symbol == nullptr)
        {
            printf("Unknown type: %s\n", typeAstNode.name.c_str());
            return nullptr;;
        }

        const auto* constellationSymbol = dynamic_cast<ConstellationSymbol*>(symbol.get());

        if (constellationSymbol == nullptr)
        {
            printf("%s is not a type\n", typeAstNode.name.c_str());
            return nullptr;
        }

        return constellationSymbol->type;
    }
};
