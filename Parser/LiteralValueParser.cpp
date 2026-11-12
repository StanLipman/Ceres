#include "LiteralValueParser.h"
#include "ParserManager.h"
#include "Models/Token.h"


std::shared_ptr<LiteralValueAstNode> LiteralValueParser::Parse() const
{
    const Token literalValueToken = parser->GetNextToken();
    assert(IsLiteralValueTokenKind(literalValueToken.kind));

    if (literalValueToken.kind == TokenKind::_string || literalValueToken.kind == TokenKind::_char)
    {
        return ParseStringLiteral(literalValueToken);
    }

    return ParseNumericLiteral(literalValueToken);
}

std::shared_ptr<LiteralValueAstNode> LiteralValueParser::ParseStringLiteral(const Token& literalStringToken) const
{
    assert(literalStringToken.kind == TokenKind::_char || literalStringToken.kind == TokenKind::_string);

    if (literalStringToken.kind == TokenKind::_char && literalStringToken.text.size() > 1)
    {
        parser->GiveErrorByMessage(literalStringToken, "Char must be 1 character");
        return nullptr;
    }

    auto stringAstNode = std::make_shared<LiteralStringValueAstNode>(literalStringToken);
    stringAstNode->value = literalStringToken.text;

    return stringAstNode;
}

std::shared_ptr<LiteralValueAstNode> LiteralValueParser::ParseNumericLiteral(const Token& literalToken)
{
    assert(IsLiteralValueTokenKind(literalToken.kind) && literalToken.kind != TokenKind::_char && literalToken.kind != TokenKind::_string);

    const auto numericAstNode = std::make_shared<LiteralNumericValueAstNode>(literalToken);

    switch (literalToken.kind)
    {
    case TokenKind::_bool:
        numericAstNode->value._bool = literalToken.text == "true";
        break;
    case TokenKind::_byte:
        numericAstNode->value._byte = static_cast<std::byte>(std::stoi(literalToken.text));
        break;
    case TokenKind::_double:
        numericAstNode->value._double = std::stod(literalToken.text);
        break;
    case TokenKind::_float:
        numericAstNode->value._float = std::stof(literalToken.text);
        break;
    case TokenKind::_short:
        numericAstNode->value._short = static_cast<int16_t>(std::stoi(literalToken.text));
        break;
    case TokenKind::_ushort:
        numericAstNode->value._ushort = static_cast<uint16_t>(std::stoul(literalToken.text));
        break;
    case TokenKind::_int:
        numericAstNode->value._int = std::stoi(literalToken.text);
        break;
    case TokenKind::_uint:
        numericAstNode->value._uint = std::stoul(literalToken.text);
        break;
    case TokenKind::_long:
        numericAstNode->value._long = std::stol(literalToken.text);
        break;
    case TokenKind::_ulong:
        numericAstNode->value._ulong = std::stoul(literalToken.text);
        break;
    default:
    case TokenKind::none:
        assert(false); // Not allowed
        break;
    }

    return numericAstNode;
}
