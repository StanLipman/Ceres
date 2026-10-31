#pragma once

enum class TypeKind {
    vacuum,
    _int,
    _float,
    _bool,
    _string,
    constellation,
    pointer,
    array,
    method,
};

struct Type
{
    TypeKind kind;
    virtual ~Type() = default;
};

struct PrimitiveType : Type {
    // Integer, Float, Boolean, etc.
};

struct ClassType : Type {
    // ClassSymbol* declaration;
};
