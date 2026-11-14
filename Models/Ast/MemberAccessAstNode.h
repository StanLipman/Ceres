#pragma once
#include <memory>

#include "AstNodes.h"
#include "Models/Token.h"

class MemberAccessAstNode : public AstNode
{
public:
    Token token;
    std::shared_ptr<MemberAccessAstNode> object; // This holds IdentifierAstNodes, of the galaxy or constellation name
    std::string member; // The name of the member to access. In cases of galaxies, this can be constellations. And in cases of constellations, this can be methods and fields.

    void accept(AstVisitor& visitor) override;
};
