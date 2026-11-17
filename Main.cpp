#include <iostream>
#include <cstring>

#include "FileIO/WriteDllFile.h"
#include "Lexer/Lexer.h"
#include "Parser/ParserManager.h"
#include "Passes/PrintAstPass.h"
#include "Passes/SemanticAnalyzer.h"
#include "MetaData/CreateMetaData.h"

int main(const int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("To few arguments provided. Please provide a valid path to a .crs file\n");
        return -1;
    }

    CompilerContext compilerContext;

    for (size_t i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "--printAst") == 0)
            compilerContext.printAst = true;
        else if (strcmp(argv[i], "--WriteDll") == 0)
            compilerContext.writeDll = true;
    }

    Lexer lexer;
    lexer.Lex(std::string(argv[1]));

    ParseManager parseManager(&compilerContext, lexer.GetTokens());

    parseManager.BuildAst();

    const auto documentNode = parseManager.GetDocumentAstNode();
    if (documentNode == nullptr)
    {
        printf("Error during parsing. No document could be parsed.\n\n");

        for (const auto& compilationError : compilerContext.compilationErrors)
            compilationError.PrintError();

        return 1;
    }

    if (compilerContext.printAst)
    {
        PrintAstPass printAstPass;
        printAstPass.visit(*documentNode);
    }

    const SemanticAnalyzer semanticAnalyzer(&compilerContext);
    semanticAnalyzer.analyze(parseManager.GetDocumentAstNode());

    for (const auto& compilationError : compilerContext.compilationErrors)
        compilationError.PrintError();

    if (compilerContext.writeDll)
    {
        std::vector<uint8_t> bytes;

        if (!CreateMetaDataFromParseManager(parseManager, bytes))
            return 1;

        if (!WriteAllBytesToNewDll(argv[1], bytes))
            return 1;

        std::cout << "Successfully created DLL" << std::endl;
    }

    return 0;
}
