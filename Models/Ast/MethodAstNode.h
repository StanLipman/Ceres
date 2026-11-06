#pragma once
#include <memory>

#include "ArgumentAstNode.h"
#include "AstNodes.h"
#include "BodyAstNode.h"
#include "TypeAstNode.h"
#include "Models/Token.h"
#include "Models/Scopes/Scope.h"

// TODO: Should this get a cool space name as well?
class MethodAstNode : public AstNode
{
public:
    TokenKind accessModifier = TokenKind::none; // TODO: Make custom enum for access modifiers
    std::string name;
    std::shared_ptr<BodyAstNode> bodyAstNode;
    std::shared_ptr<TypeAstNode> returnTypeAstNode;
    std::vector<std::shared_ptr<ArgumentAstNode>> argumentAstNodes;
    std::shared_ptr<Scope> scope;
    bool isEternal = false;

    void accept(AstVisitor& visitor) override;
};
