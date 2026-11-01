#pragma once
#include <iostream>
#include <cstring>

#include "FileIO/WriteDllFile.h"
#include "Lexer/Lexer.h"
#include "Parser/ParserManager.h"
#include "MetaData/CreateMetaData.h"

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
            std::vector<uint8_t> bytes;
            bytes.reserve(1024 * 1024); // Reserve 1 MB

            if (!CreateMetaDataFromParseManager(parseManager, bytes))
                return 1;

            if (!WriteAllBytesToNewDll(argv[1], bytes))
                return 1;

            std::cout << "Succesfully created DLL" << std::endl;
        }
    }

    return 0;
}
