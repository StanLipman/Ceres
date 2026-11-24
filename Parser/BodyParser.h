#pragma once
#include "Models/Ast/BodyAstNode.h"
#include "Models/Ast/MethodAstNode.h"

class ParseManager;

class BodyParser
{
private:
    ParseManager* parser;

public:
    explicit BodyParser(ParseManager* parser): parser(parser) {}

    [[nodiscard]] std::shared_ptr<BodyAstNode> ParseGalaxyBody() const;
    [[nodiscard]] std::shared_ptr<BodyAstNode> ParseConstellationBody() const;
    [[nodiscard]] std::shared_ptr<BodyAstNode> ParseMethodBody(const std::shared_ptr<MethodAstNode>& parent) const;
};
