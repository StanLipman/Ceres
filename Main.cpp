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

    Lexer lexer;
    lexer.Lex(std::string(argv[1]));
    ParseManager parseManager(lexer.GetTokens());

    parseManager.BuildAst();

    for (size_t i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "--printAst") == 0)
        {
            parseManager.PrintAst();
        }
        else if (strcmp(argv[i], "--WriteDll") == 0)
        {
            // Test data
            std::vector<uint8_t> bytes = { 0x4D, 0x5A, 0x90, 0x00 };

            if (!WriteAllBytesToNewDll(argv[1], bytes))
                return 1;

            std::cout << "Succesfully created DLL" << std::endl;
        }
    }

    return 0;
}
