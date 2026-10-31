#pragma once
#include <iostream>
#include "FileIO/WriteDllFile.h"
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

    // Test data
    std::vector<uint8_t> bytes = { 0x4D, 0x5A, 0x90, 0x00 };
    Lexer lexer;
    lexer.Lex(std::string(argv[1]));

    if (!WriteAllBytesToNewDll(argv[1], bytes))
        return 1;
    ParseManager parseManager(lexer.GetTokens());

    std::cout << "Succesfully created DLL" << std::endl;
    parseManager.BuildAst();

    if (argc == 3 && strcmp(argv[2], "--printAst") == 0)
    {
        parseManager.PrintAst();
    }
    return 0;
}
