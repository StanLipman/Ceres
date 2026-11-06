#pragma once

// Forward declare types to prevent having circular file references
class DocumentAstNode;
class GalaxyAstNode;
class ArgumentAstNode;
class BodyAstNode;
class ConstellationAstNode;
class MethodAstNode;
class TypeAstNode;

class AstVisitor {
    public:
    virtual ~AstVisitor() = default;

    virtual void visit(DocumentAstNode& node) = 0;
    virtual void visit(GalaxyAstNode& node) = 0;
    virtual void visit(ArgumentAstNode& node) = 0;
    virtual void visit(BodyAstNode& node) = 0;
    virtual void visit(ConstellationAstNode& node) = 0;
    virtual void visit(MethodAstNode& node) = 0;
    virtual void visit(TypeAstNode& node) = 0;
};
