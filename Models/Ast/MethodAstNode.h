#pragma once
#include <memory>

#include "ArgumentAstNode.h"
#include "AstNodes.h"
#include "BodyAstNode.h"
#include "TypeAstNode.h"
#include "Models/Token.h"

// TODO: Should this get a cool space name as well?
class MethodAstNode : public AstNode
{
private:
    TokenKind accessModifier = TokenKind::none; // TODO: Make custom enum for access modifiers
    std::string identifierName;
    std::shared_ptr<BodyAstNode> bodyAstNode;
    std::shared_ptr<TypeAstNode> returnTypeAstNode;
    std::vector<std::shared_ptr<ArgumentAstNode>> argumentAstNodes;
    bool isEternal = false;

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

    void SetReturnType(const std::shared_ptr<TypeAstNode>& returnType)
    {
        assert(returnTypeAstNode == nullptr);
        this->returnTypeAstNode = returnType;
    }

    std::shared_ptr<TypeAstNode> GetReturnType()
    {
        return returnTypeAstNode;
    }

    void SetIsEternal()
    {
        assert(!isEternal);
        isEternal = true;
    }

    bool IsEternal()
    {
        return this->isEternal;
    }

    void AddArgument(const std::shared_ptr<ArgumentAstNode>& argument)
    {
        argumentAstNodes.emplace_back(argument);
    }

    std::vector<std::shared_ptr<ArgumentAstNode>>& GetArguments()
    {
        return argumentAstNodes;
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

    void accept(AstVisitor& visitor) override;
};
