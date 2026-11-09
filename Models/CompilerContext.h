#pragma once
#include <cstdint>

using SymbolId = uint64_t;
using TypeId = uint64_t;

class CompilerContext
{
private:
    SymbolId nextSymbolId = 1;
    TypeId nextTypeId = 1;

public:

    SymbolId GetNextSymbolId()
    {
        return nextSymbolId++;
    }

    TypeId GetNextTypeId()
    {
        return nextTypeId++;
    }
};
