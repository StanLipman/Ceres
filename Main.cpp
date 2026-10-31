#pragma once
#include <cstring>

#include "Lexer/Lexer.h"
#include "Parser/ParserManager.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("To few arguments provided. Please provide a valid path to a .crs file\n");
        return -1;
    }

    Lexer lexer;
    lexer.Lex(std::string(argv[1]));

    ParseManager parseManager(lexer.GetTokens());

    parseManager.BuildAst();

    if (argc == 3 && strcmp(argv[2], "--printAst") == 0)
    {
        parseManager.PrintAst();
    }
    return 0;
}
