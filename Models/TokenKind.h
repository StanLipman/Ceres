#pragma once
#include <cassert>
#include <unordered_map>
#include <unordered_set>

#define ACCESS_MODIFIER_TOKENS(X) \
    X(universal, "universal") /* public */ \
    X(local, "local") /* private */ \
    X(shielded, "shielded") /* protected */ \
    X(enclosed, "enclosed") /* internal */

#define TOKENS(X) \
    /* Access Modifiers and Declarations */ \
    X(constellation, "constellation") /* class */ \
    X(world, "world") /* struct */ \
    X(atlas, "atlas") /* record */ \
    X(protocol, "protocol") /* interface */ \
    X(spectrum, "spectrum") /* enum */ \
    X(galaxy, "galaxy") /* namespace */ \
    X(_import, "import") /* using */ \
    X(forge, "forge") /* new */ \
    X(self, "self") /* this */ \
    X(origin, "origin") /* base */ \
    /* Types and Modifiers */ \
    X(eternal, "eternal") /* static */ \
    X(ethereal, "ethereal") /* abstract */  \
    X(fluid, "fluid") /* virtual */ \
    X(transform, "transform") /* override */ \
    X(bound, "bound") /* sealed */ \
    X(secured, "secured") /* readonly */ \
    X(absolute, "absolute") /* const */ \
    X(parallel, "parallel") /* async */ \
    X(converge, "converge") /* await */ \
    X(vacuum, "vacuum") /* void */ \
    X(absence, "absence") /* null */ \
    X(_auto, "auto") /* var */ \
    /* Syntax and Operators */ \
    X(lParen, "(") \
    X(rParen, ")") \
    X(lCurly, "{") \
    X(rCurly, "}") \
    X(semicolon, ";") \
    X(comma, ",")

#define SPECIAL_TOKENS(X) \
    X(identifier, "identifier") \
    X(eof, "eof")

enum class TokenKind {
    none = 0, // To check for accidental assignment
#define ENUM_ENTRY(name, value) name,
    ACCESS_MODIFIER_TOKENS(ENUM_ENTRY)
    TOKENS(ENUM_ENTRY)
    SPECIAL_TOKENS(ENUM_ENTRY)
#undef ENUM_ENTRY
};

static std::unordered_map<std::string, TokenKind> tokenKindMap {
#define DICT_ENTRY(name, value) {value, TokenKind::name},
    ACCESS_MODIFIER_TOKENS(DICT_ENTRY)
    TOKENS(DICT_ENTRY)
#undef DICT_ENTRY
};

static std::unordered_set accessModifierTokens {
#define DICT_ENTRY(name, value) TokenKind::name,
    ACCESS_MODIFIER_TOKENS(DICT_ENTRY)
#undef DICT_ENTRY
};

static std::unordered_map<TokenKind, std::string> tokenDisplayName {
#define DICT_ENTRY(name, value) {TokenKind::name, value},
    ACCESS_MODIFIER_TOKENS(DICT_ENTRY)
    TOKENS(DICT_ENTRY)
    SPECIAL_TOKENS(DICT_ENTRY)
#undef DICT_ENTRY
};

static bool IsAccessModifier(const TokenKind kind)
{
    return accessModifierTokens.contains(kind);
}

static std::string GetTokenKindText(const TokenKind kind)
{
    assert(tokenDisplayName.contains(kind));
    return tokenDisplayName[kind];
}
