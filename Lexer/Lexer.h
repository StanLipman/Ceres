#pragma once

#include <fstream>
#include <vector>

#include "Models/Token.h"

class Lexer
{
private:
    std::vector<Token> tokens;

public:
    void Lex(const std::string& fileName);
    std::vector<Token> GetTokens()
    {
        return tokens;
    }


private:
    void LexLine(const std::string& line, size_t lineNr);
    void HandleCompleteToken(std::string& tokenText, TokenKind tokenKind, size_t lineNr, size_t startCol);
};
