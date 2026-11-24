#pragma once
#include <memory>

#include "AstVisitor.h"
#include "AutoResolver.h"
#include "Models/Ast/AutoAstNode.h"
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

    void visit(MethodCallAstNode& node) override
    {
        // Skip
    }

    void visit(MemberAccessAstNode& node) override
    {
        // Skip
    }

    void visit(ExpressionAstNode& node) override
    {
        const auto variableDeclarationAstNode = dynamic_cast<VariableDeclarationAstNode*>(&node);
        if (variableDeclarationAstNode == nullptr)
            return;

        const auto variableSymbol = std::make_shared<VariableSymbol>();
        assert(!variableDeclarationAstNode->name.empty());
        assert(variableDeclarationAstNode->type != nullptr);
        variableSymbol->name = variableDeclarationAstNode->name;

        if (const auto& typeAstNode = std::dynamic_pointer_cast<TypeAstNode>(variableDeclarationAstNode->type); typeAstNode != nullptr)
        {
            variableSymbol->type = ResolveType(*typeAstNode);

            if (variableSymbol->type == nullptr)
                return; // Error has been handled in ResolveType

            if (currentScope->Insert(variableSymbol))
                return;

            std::string formattedMessage = "Duplicate variable '" + variableDeclarationAstNode->name + "'";
            compilerContext->compilationErrors.emplace_back(formattedMessage, typeAstNode->token.location);
            return;
        }

        const auto& autoAstNode = std::dynamic_pointer_cast<AutoAstNode>(variableDeclarationAstNode->type);
        assert(autoAstNode != nullptr);
        assert(!autoAstNode->token.text.empty());

        const auto& resolvedType = AutoResolver::ResolveType(currentScope, *variableDeclarationAstNode);
        assert(resolvedType != nullptr);

        variableSymbol->type = resolvedType;

        if (currentScope->Insert(variableSymbol))
            return;

        std::string formattedMessage = "Duplicate variable '" + variableDeclarationAstNode->name + "'";
        compilerContext->compilationErrors.emplace_back(formattedMessage, autoAstNode->token.location);
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
