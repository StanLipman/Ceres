#pragma once
#include <memory>
#include <string>

#include "Models/CompilerContext.h"
#include "Models/Types/Type.h"

class Symbol
{
public:
    SymbolId symbolId;
    std::string name;

    virtual ~Symbol() = default;
};

class VariableSymbol : public Symbol
{
public:
    std::shared_ptr<Type> type;
};