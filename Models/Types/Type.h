#pragma once
#include <cstdint>
#include <memory>
#include <vector>

class Type
{
public:
    enum TypeKind
    {
        _bool,
        _byte,
        _char,
        _double,
        _float,
        _short,
        _ushort,
        _int,
        _uint,
        _long,
        _ulong,
        string,
        vacuum,
        constellation,
        method,
    };

    TypeId id;
    TypeKind kind;

    virtual ~Type() = default;
};

class ConstellationType : public Type
{
public:
    SymbolId declarationSymbolId;
};

class MethodType : public Type
{
public:
    std::shared_ptr<Type> returnType;
    std::vector<std::shared_ptr<Type>> parameterTypes;
};