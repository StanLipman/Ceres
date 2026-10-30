#pragma once
#include <cassert>
#include <string>
#include <unordered_map>

enum class TokenKind {
    none = 0, // To check for accidental assignment

    // Access Modifiers and Declarations
    universal, // public
    local, // private
    shielded, // protected
    enclosed, // internal
    constellation, // class
    world, // struct
    atlas, // record
    protocol, // interface
    spectrum, // enum
    galaxy, // namespace
    import, // using
    forge, // new
    self, // this
    origin, // base

    // Types and Modifiers
    eternal, // static
    ethereal, // abstract
    fluid, // virtual
    transform, // override
    bound, // sealed
    secured, // readonly
    absolute, // const
    parallel, // async
    converge, // await
    vacuum, // void
    absence, // null
    _auto, // var

    // Syntax and Operators
    lParen,
    rParen,
    lCurly,
    rCurly,
    semicolon,
    identifier, // Excluded from tokenKindMap as it does not have a predefined shape
    eof, // Excluded from tokenKindMap as it does not have a predefined shape
};

static std::unordered_map<std::string, TokenKind> tokenKindMap {
    // Access Modifiers and Declarations
    {"universal", TokenKind::universal},
    {"local", TokenKind::local},
    {"shielded", TokenKind::shielded},
    {"enclosed", TokenKind::enclosed},
    {"constellation", TokenKind::constellation},
    {"world", TokenKind::world},
    {"atlas", TokenKind::atlas},
    {"protocol", TokenKind::protocol},
    {"spectrum", TokenKind::spectrum},
    {"galaxy", TokenKind::galaxy},
    {"import", TokenKind::import},
    {"forge", TokenKind::forge},
    {"self", TokenKind::self},
    {"origin", TokenKind::origin},

    // Types and Modifiers
    {"eternal", TokenKind::eternal},
    {"ethereal", TokenKind::ethereal},
    {"fluid", TokenKind::fluid},
    {"transform", TokenKind::transform},
    {"bound", TokenKind::bound},
    {"secured", TokenKind::secured},
    {"absolute", TokenKind::absolute},
    {"parallel", TokenKind::parallel},
    {"converge", TokenKind::converge},
    {"vacuum", TokenKind::vacuum},
    {"absence", TokenKind::absence},
    {"auto", TokenKind::_auto},

    // Syntax and Operators
    {"(", TokenKind::lParen},
    {")", TokenKind::rParen},
    {"{", TokenKind::lCurly},
    {"}", TokenKind::rCurly},
    {";", TokenKind::semicolon}
};

struct SourceLocation {
    size_t line;
    size_t column;
};

struct Token {
    TokenKind kind;
    std::string text;
    SourceLocation location;
};

static bool IsAccessModifier(const TokenKind kind)
{
    switch (kind) {
    case TokenKind::universal:
    case TokenKind::local:
    case TokenKind::shielded:
    case TokenKind::enclosed:
        return true;
    case TokenKind::none:
        assert(false); // Is not allowed
    case TokenKind::constellation:
    case TokenKind::galaxy:
    case TokenKind::eternal:
    case TokenKind::vacuum:
    case TokenKind::lParen:
    case TokenKind::rParen:
    case TokenKind::lCurly:
    case TokenKind::rCurly:
    case TokenKind::semicolon:
    case TokenKind::identifier:
    case TokenKind::world:
    case TokenKind::atlas:
    case TokenKind::protocol:
    case TokenKind::spectrum:
    case TokenKind::import:
    case TokenKind::forge:
    case TokenKind::self:
    case TokenKind::origin:
    case TokenKind::ethereal:
    case TokenKind::fluid:
    case TokenKind::transform:
    case TokenKind::bound:
    case TokenKind::secured:
    case TokenKind::absolute:
    case TokenKind::parallel:
    case TokenKind::converge:
    case TokenKind::absence:
    case TokenKind::_auto:
    case TokenKind::eof:
    default:
        return false;
    }
}

static std::string GetTokenKindText(const TokenKind kind)
{
    switch (kind) {
    case TokenKind::none:
        assert(false);
        return "none";
    case TokenKind::universal:
        return "universal";
    case TokenKind::local:
        return "local";
    case TokenKind::shielded:
        return "shielded";
    case TokenKind::enclosed:
        return "enclosed";
    case TokenKind::constellation:
        return "constellation";
    case TokenKind::world:
        return "world";
    case TokenKind::atlas:
        return "atlas";
    case TokenKind::protocol:
        return "protocol";
    case TokenKind::spectrum:
        return "spectrum";
    case TokenKind::galaxy:
        return "galaxy";
    case TokenKind::import:
        return "import";
    case TokenKind::forge:
        return "forge";
    case TokenKind::self:
        return "self";
    case TokenKind::origin:
        return "origin";
    case TokenKind::eternal:
        return "eternal";
    case TokenKind::ethereal:
        return "ethereal";
    case TokenKind::fluid:
        return "fluid";
    case TokenKind::transform:
        return "transform";
    case TokenKind::bound:
        return "bound";
    case TokenKind::secured:
        return "secured";
    case TokenKind::absolute:
        return "absolute";
    case TokenKind::parallel:
        return "parallel";
    case TokenKind::converge:
        return "converge";
    case TokenKind::vacuum:
        return "vacuum";
    case TokenKind::absence:
        return "absence";
    case TokenKind::_auto:
        return "auto";
    case TokenKind::lParen:
        return "(";
    case TokenKind::rParen:
        return ")";
    case TokenKind::lCurly:
        return "{";
    case TokenKind::rCurly:
        return "}";
    case TokenKind::semicolon:
        return ";";
    case TokenKind::identifier:
        return "identifier";
    case TokenKind::eof:
        return "eof";
    default:
        assert(false);
        return "unknown";
    }
}
