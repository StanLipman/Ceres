#pragma once
#include <memory>

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

public:
    void SetName(const std::string& galaxyName)
    {
        this->identifierName = galaxyName;
    }

    void SetAccessModifier(const TokenKind tokenKind)
    {
        assert(IsAccessModifier(tokenKind));
        this->accessModifier = tokenKind;
    }

    void SetReturnType(const std::shared_ptr<TypeAstNode>& returnType)
    {
        assert(returnTypeAstNode == nullptr);
        this->returnTypeAstNode = returnType;
    }

    void SetBody(const std::shared_ptr<BodyAstNode>& body)
    {
        assert(this->bodyAstNode == nullptr);
        this->bodyAstNode = body;
    }

    void Print(const int depth) override
    {
        for (size_t i = 0; i < depth; i++)
            printf("-");

        printf("%s ", GetTokenKindText(accessModifier).c_str());
        returnTypeAstNode->Print(depth);
        printf(" %s()", identifierName.c_str());
        bodyAstNode->Print(depth + 1);
        printf("\n");
    }
};
