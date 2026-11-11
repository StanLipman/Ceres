#pragma once
#include <memory>

#include "AstVisitor.h"
#include "Models/Ast/DocumentAstNode.h"
#include "Models/Ast/TransmitAstNode.h"
#include "Models/Scopes/Scope.h"
#include "Models/Symbols/MethodSymbol.h"

class DeclarationResolver: AstVisitor
{
private:
    std::shared_ptr<Scope> currentScope;
    CompilerContext* compilerContext;

public:
    explicit DeclarationResolver(const std::shared_ptr<Scope>& globalScope, CompilerContext* compilerContext) : currentScope(globalScope), compilerContext(compilerContext) {}

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
            std::string formattedMessage = "Missing method symbol";
            compilerContext->compilationErrors.emplace_back(formattedMessage, node.token.location);
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
                std::string formattedMessage = "Duplicate parameter '" + argumentAstNode->name + "'";
                compilerContext->compilationErrors.emplace_back(formattedMessage, argumentAstNode->typeAstNode->token.location);
                continue;
            }
        }

        assert(node.returnTypeAstNode != nullptr);
        node.returnTypeAstNode->accept(*this);

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

    void visit(TransmitAstNode& node) override
    {
        // Skip
    }

    void visit(LiteralValueAstNode& node) override
    {
        // Skip
    }

    void visit(VariableAstNode& node) override
    {
        // skip
    }

private:
    std::shared_ptr<Type> ResolveType(const TypeAstNode& typeAstNode) const
    {
        assert(!typeAstNode.name.empty());
        const std::shared_ptr<Symbol> symbol = currentScope->Lookup(typeAstNode.name);

        if (symbol == nullptr)
        {
            std::string formattedMessage = "Unknown type: " + typeAstNode.name;
            compilerContext->compilationErrors.emplace_back(formattedMessage, typeAstNode.token.location);
            return nullptr;;
        }

        const auto* constellationSymbol = dynamic_cast<ConstellationSymbol*>(symbol.get());

        if (constellationSymbol == nullptr)
        {
            std::string formattedMessage = typeAstNode.name + "is not a type";
            compilerContext->compilationErrors.emplace_back(formattedMessage, typeAstNode.token.location);
            return nullptr;
        }

        return constellationSymbol->type;
    }
};
