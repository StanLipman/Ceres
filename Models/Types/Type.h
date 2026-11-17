#pragma once
#include <cstdint>
#include <memory>
#include <vector>

class Type
{
public:
    enum TypeKind
    {
        none = 0,
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
    TypeKind kind = none;

    virtual ~Type() = default;

    [[nodiscard]] std::string GetTypeKindText() const
    {
        switch (kind) {
        case _bool:
            return "bool";
        case _byte:
            return "byte";
        case _char:
            return "char";
        case _double:
            return "double";
        case _float:
            return "float";
        case _short:
            return "short";
        case _ushort:
            return "ushort";
        case _int:
            return "int";
        case _uint:
            return "uint";
        case _long:
            return "long";
        case _ulong:
            return "ulong";
        case string:
            return "string";
        case vacuum:
            return "vacuum";
        case constellation:
            return "constellation";
        case method:
            return "method";
        default:
            assert(false);
            return "unknown type";
        }
    }
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