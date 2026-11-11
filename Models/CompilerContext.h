#pragma once
#include <cstdint>
#include <vector>
#include "./Models/Token.h"

using SymbolId = uint64_t;
using TypeId = uint64_t;

struct CompilationError
{
    std::string formattedMessage;
    SourceLocation location;

    void PrintError() const
    {
        printf("%s:%lu:%lu\n\t%s\n", location.filePath.c_str(), location.line, location.column, formattedMessage.c_str());
    }
};

class CompilerContext
{
private:
    SymbolId nextSymbolId = 1;
    TypeId nextTypeId = 1;
public:
    std::vector<CompilationError> compilationErrors;
    bool printAst = false;
    bool writeDll = false;

    SymbolId GetNextSymbolId()
    {
        return nextSymbolId++;
    }

    TypeId GetNextTypeId()
    {
        return nextTypeId++;
    }
};
