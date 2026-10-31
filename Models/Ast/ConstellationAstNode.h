#pragma once
#include <memory>

#include "AstNodes.h"
#include "BodyAstNode.h"
#include "Models/Token.h"

class ConstellationAstNode : public AstNode
{
private:
    TokenKind accessModifier = TokenKind::none; // TODO: Make custom enum for access modifiers
    std::string identifierName;
    std::shared_ptr<BodyAstNode> bodyAstNode;

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

    void SetBody(const std::shared_ptr<BodyAstNode>& body)
    {
        assert(this->bodyAstNode == nullptr);
        this->bodyAstNode = body;
    }

    void Print(const int depth) override
    {
        for (size_t i = 0; i < depth; i++)
            printf("-");
        printf("%s constellation %s\n", GetTokenKindText(accessModifier).c_str(), this->identifierName.c_str());

        if (bodyAstNode == nullptr)
            return;

        bodyAstNode->Print(depth + 1);
    }
};
