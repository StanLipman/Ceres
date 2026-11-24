#pragma once
#include <string>

#include "TokenKind.h"

struct SourceLocation {
    std::string filePath;
    size_t line;
    size_t column;

    bool operator==(const SourceLocation& location) const = default;
};

struct Token {
    TokenKind kind;
    std::string text;
    SourceLocation location;
    bool operator==(const Token& token) const
    {
        return this->kind == token.kind &&
               this->text == token.text &&
               this->location == token.location;
    };
};

static Token EmptyToken = {
    .kind = TokenKind::none,
    .text = "",
    .location = {
        .filePath = "",
        .line = 0,
        .column = 0,
    }
};

