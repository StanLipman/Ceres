#pragma once

#include <cstdint>

#include "AstNodes.h"
#include "Models/TokenKind.h"

class LiteralValueAstNode : public AstNode
{
public:
    TokenKind tokenKind = TokenKind::none;

    explicit LiteralValueAstNode(const TokenKind tokenKind)
    {
        this->tokenKind = tokenKind;
    }

    void accept(AstVisitor& visitor) override;
};

class LiteralStringValueAstNode : public LiteralValueAstNode {
public:
    std::string value;

    explicit LiteralStringValueAstNode(const TokenKind tokenKind) : LiteralValueAstNode(tokenKind) { }
};

class LiteralNumericValueAstNode : public LiteralValueAstNode
{
public:
    union
    {
        bool _bool;
        std::byte _byte;
        char _char;
        double _double;
        float _float;
        int16_t _short;
        uint16_t _ushort;
        int32_t _int;
        uint32_t _uint;
        int64_t _long;
        uint64_t _ulong;
    } value;

    explicit LiteralNumericValueAstNode(const TokenKind tokenKind) : LiteralValueAstNode(tokenKind), value(false) { }
};
