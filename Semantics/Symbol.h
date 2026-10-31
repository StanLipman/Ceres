#pragma once
#include <string>

#include "TypeSystem/Type.h"

enum class SymbolKind {
    variable,
    parameter,
    method,
    constellation,
    galaxy,
};

struct Symbol {
    std::string name;
    SymbolKind kind;
    Type* type;
};
