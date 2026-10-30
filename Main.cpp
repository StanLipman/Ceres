#pragma once
#include "Lexer/Lexer.h"
#include "Parser/ParserManager.h"

int main(int argc, char* argv[])
{
    Lexer lexer;
    lexer.Lex(std::string(argv[1]));

    ParseManager parseManager(lexer.GetTokens());
    parseManager.BuildAst();
    return 0;
}
