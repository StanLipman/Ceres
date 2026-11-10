#pragma once
#include <string>

#include "TokenKind.h"

struct SourceLocation {
    std::string filePath;
    size_t line;
    size_t column;
};

struct Token {
    TokenKind kind;
    std::string text;
    SourceLocation location;
};

static Token EmptyToken = {
    .kind = TokenKind::none,
    .text = "",
    .location = {
        "",
        0,
        0,
    }
};

