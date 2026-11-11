#include "LiteralValueParser.h"

#include "ParserManager.h"
#include "Models/Token.h"
#include "Models/Ast/TransmitAstNode.h"

void LiteralStringValueParser::Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<LiteralStringValueAstNode>& stringValueAstNode)
{
    const auto transmitAstNode = std::dynamic_pointer_cast<TransmitAstNode>(parent);
    assert(transmitAstNode != nullptr);
    const Token literalToken = parser->GetNextToken();

    if (literalToken.kind == TokenKind::_char && literalToken.text.size() > 1)
    {
        parser->GiveErrorByMessage(literalToken, "Char must be 1 character");
        return;
    }

    assert(stringValueAstNode->value.empty());
    stringValueAstNode->value = literalToken.text;

    transmitAstNode->transmitValue = stringValueAstNode;
}

void LiteralNumericValueParser::Parse(const std::shared_ptr<AstNode> parent, const std::shared_ptr<LiteralNumericValueAstNode>& numericValueAstNode)
{
    const Token literalToken = parser->GetNextToken();

    const auto transmitAstNode = std::dynamic_pointer_cast<TransmitAstNode>(parent);
    assert(transmitAstNode != nullptr);
    if (literalToken.kind == TokenKind::_bool)
    {
        numericValueAstNode->value._bool = literalToken.text == "true";
    }
    else if (literalToken.kind == TokenKind::_byte)
    {
        numericValueAstNode->value._byte = static_cast<std::byte>(std::stoi(literalToken.text));
    }
    else if (literalToken.kind == TokenKind::_double)
    {
        numericValueAstNode->value._double = std::stod(literalToken.text);
    }
    else if (literalToken.kind == TokenKind::_float)
    {
        numericValueAstNode->value._float = std::stof(literalToken.text);
    }
    else if (literalToken.kind == TokenKind::_short)
    {
        numericValueAstNode->value._short = static_cast<int16_t>(std::stoi(literalToken.text));
    }
    else if (literalToken.kind == TokenKind::_ushort)
    {
        numericValueAstNode->value._ushort = static_cast<uint16_t>(std::stoul(literalToken.text));
    }
    else if (literalToken.kind == TokenKind::_int)
    {
        numericValueAstNode->value._int = std::stoi(literalToken.text);
    }
    else if (literalToken.kind == TokenKind::_uint)
    {
        numericValueAstNode->value._uint = std::stoul(literalToken.text);
    }
    else if (literalToken.kind == TokenKind::_long)
    {
        numericValueAstNode->value._long = std::stol(literalToken.text);
    }
    else if (literalToken.kind == TokenKind::_ulong)
    {
        numericValueAstNode->value._ulong = std::stoul(literalToken.text);
    }
    else
    {
        assert(false); // What happened?
    }

    transmitAstNode->transmitValue = numericValueAstNode;
}
