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
        const auto& returnType = methodType->returnType;
        assert(returnType != nullptr);

        for (const auto& statementAstNode : node.bodyAstNode->statementAstNodes)
        {
            if (const auto* transmitStatement = dynamic_cast<TransmitAstNode*>(statementAstNode.get()); transmitStatement != nullptr)
            {
                const auto& transmitValue = transmitStatement->transmitValue;

                const std::shared_ptr<Type>& transmitValueType = GetType(node.scope, transmitValue);
                if (transmitValueType != returnType)
                {
                    std::string formattedMessage;
                    SourceLocation location;

                    if (transmitValue == nullptr)
                    {
                        formattedMessage = "Method '" + node.name + "' expected transmit type of '" + node.returnTypeAstNode->name + "', but got empty transmit.";
                        location = transmitStatement->token.location;
                    }
                    else if (const auto& literalValue = std::dynamic_pointer_cast<LiteralValueAstNode>(transmitValue); literalValue != nullptr)
                    {
                        const TokenKind transmitValueTokenKind = literalValue->tokenKind;
                        formattedMessage = "Method '" + node.name + "' expected transmit type of '" + node.returnTypeAstNode->name + "', but '" + GetTokenKindText(transmitValueTokenKind) + "' got transmitted.";
                        location = literalValue->token.location;
                    }
                    else if (const auto& variable = std::dynamic_pointer_cast<VariableAstNode>(transmitValue); variable != nullptr)
                    {
                        if (transmitValueType == nullptr)
                        {
                            formattedMessage = "Tried to transmit undefined value of name '" + variable->name + "'";
                            location = transmitStatement->token.location;
                        }
                        else
                        {
                            formattedMessage = "Method '" + node.name + "' expected transmit type of '" + node.returnTypeAstNode->name + "', but '" + transmitValueType->GetTypeKindText() + "' got transmitted.";
                            location = variable->token.location;
                        }
                    }

                    compilerContext->compilationErrors.emplace_back(formattedMessage, location);
                    return;
                }
            }
            else if (const auto* methodCallStatement = dynamic_cast<MethodCallAstNode*>(statementAstNode.get()); methodCallStatement != nullptr)
            {
                const auto& symbol = node.scope->Lookup(methodCallStatement->name);
                const auto callingMethodSymbol = std::dynamic_pointer_cast<MethodSymbol>(symbol);
                assert(callingMethodSymbol != nullptr);
                if (callingMethodSymbol == nullptr)
                {
                    const std::string formattedMessage = "Method '" + methodCallStatement->name + "' does not exist.";
                    const SourceLocation location = methodCallStatement->token.location;
                    compilerContext->compilationErrors.emplace_back(formattedMessage, location);
                    return;
                }

                if (callingMethodSymbol->type->parameterTypes.size() != methodCallStatement->arguments.size())
                {
                    const std::string formattedMessage = "Method '" + methodCallStatement->name + "' got " + std::to_string(methodCallStatement->arguments.size()) + " arguments, but expected " + std::to_string(callingMethodSymbol->type->parameterTypes.size()) + " arguments.";
                    const SourceLocation location = methodCallStatement->token.location;
                    compilerContext->compilationErrors.emplace_back(formattedMessage, location);
                    return;
                }

                for (size_t i = 0; i < methodCallStatement->arguments.size(); i++)
                {
                    if (i > callingMethodSymbol->type->parameterTypes.size())
                        break; // More arguments were provided then expected

                    const auto& providedArgument = methodCallStatement->arguments[i];
                    assert(providedArgument != nullptr);
                    const auto& providedArgumentType = GetType(node.scope, providedArgument);
                    assert(providedArgumentType != nullptr);
                    const auto& expectedArgumentType = callingMethodSymbol->type->parameterTypes[i];
                    assert(expectedArgumentType != nullptr);

                    if (providedArgumentType != expectedArgumentType)
                    {
                        assert(providedArgumentType->kind != Type::TypeKind::none);
                        assert(expectedArgumentType->kind != Type::TypeKind::none);

                        const std::string formattedMessage = "Method '" + methodCallStatement->name + "' expected argument with type of" + expectedArgumentType->GetTypeKindText() + " but got " + providedArgumentType->GetTypeKindText() + " arguments.";
                        const SourceLocation location = methodCallStatement->token.location;
                        compilerContext->compilationErrors.emplace_back(formattedMessage, location);
                    }
                }
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

    void visit(VariableAstNode& node) override
    {
        // Skip? Handled in MethodAstNode
    }

    void visit(MethodCallAstNode& node) override
    {
        // TODO: Do we need to handle this?
        // Maybe we here do the validation if the method exists, and if the passed arguments are valid
    }

private:
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
        assert(variableSymbol != nullptr);
        return variableSymbol->type;
    }
};
