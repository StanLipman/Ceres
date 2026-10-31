#pragma once
#include <string>

#include "TokenKind.h"

struct SourceLocation {
    size_t line;
    size_t column;
};

struct Token {
    TokenKind kind;
    std::string text;
    SourceLocation location;
};

