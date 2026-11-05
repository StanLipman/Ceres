#pragma once
#include <memory>

#include "AstNodes.h"
#include "BodyAstNode.h"
#include "Models/Token.h"
#include "Models/Scopes/Scope.h"

class ConstellationAstNode : public AstNode
{
private:
    TokenKind accessModifier = TokenKind::none; // TODO: Make custom enum for access modifiers
    std::string identifierName;
    std::shared_ptr<BodyAstNode> bodyAstNode;
    std::shared_ptr<Scope> scope;

public:
    void SetName(const std::string& galaxyName)
    {
        this->identifierName = galaxyName;
    }

    std::string GetName()
    {
        return this->identifierName;
    }

    void SetAccessModifier(const TokenKind tokenKind)
    {
        assert(IsAccessModifier(tokenKind));
        this->accessModifier = tokenKind;
    }

    [[nodiscard]] TokenKind GetAccessModifier() const
    {
        return this->accessModifier;
    }

    void SetBody(const std::shared_ptr<BodyAstNode>& body)
    {
        assert(this->bodyAstNode == nullptr);
        this->bodyAstNode = body;
    }

    std::shared_ptr<BodyAstNode> GetBody()
    {
        return this->bodyAstNode;
    }

    void SetScope(const std::shared_ptr<Scope>& scopeToSet)
    {
        assert(scope == nullptr);
        scope = scopeToSet;
    }

    std::shared_ptr<Scope> GetScope()
    {
        assert(scope != nullptr);
        return scope;
    }

    void accept(AstVisitor& visitor) override;
};
