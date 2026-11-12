#pragma once
#include <memory>
#include <string>
#include <vector>

#include "AstNodes.h"
#include "Models/Token.h"

class MethodCallAstNode : public AstNode
{
public:
    std::string name;
    std::vector<std::shared_ptr<AstNode>> arguments;
    std::shared_ptr<AstNode> object; // This holds either and eternal constellation, or a reference ot an object
    Token token;

    void accept(AstVisitor& visitor) override;
};
