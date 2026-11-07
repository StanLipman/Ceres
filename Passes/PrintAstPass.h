#pragma once
#include "AstVisitor.h"

class PrintAstPass: public AstVisitor
{
private:
    int currentDepth = 0;
public:
    void visit(DocumentAstNode& node) override;
    void visit(GalaxyAstNode& node) override;
    void visit(ArgumentAstNode& node) override;
    void visit(BodyAstNode& node) override;
    void visit(ConstellationAstNode& node) override;
    void visit(MethodAstNode& node) override;
    void visit(TypeAstNode& node) override;
    void visit(TransmitAstNode& node) override;
    void visit(LiteralValueAstNode& node) override;
};