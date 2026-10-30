#pragma once
#include "BaseParser.h"
#include "Models/Token.h"
#include "Models/Ast/DocumentAstNode.h"

class DocumentParser: public BaseParser<DocumentAstNode>
{
public:
    explicit DocumentParser(ParseManager* parser)
    : BaseParser(parser) { }

    void Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<DocumentAstNode>& astNode) override;
};
