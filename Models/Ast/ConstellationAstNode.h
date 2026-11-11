#pragma once
#include <memory>

#include "AstNodes.h"
#include "BodyAstNode.h"
#include "Models/Token.h"
#include "Models/Scopes/Scope.h"

class ConstellationAstNode : public AstNode
{
public:
    TokenKind accessModifier = TokenKind::none; // TODO: Make custom enum for access modifiers
    std::string name;
    std::shared_ptr<BodyAstNode> bodyAstNode;
    std::shared_ptr<Scope> scope;
    Token token = EmptyToken;

    void accept(AstVisitor& visitor) override;
};
