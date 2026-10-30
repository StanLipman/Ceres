#include "Lexer.h"

void Lexer::Lex(const std::string& fileName)
{
    std::ifstream sourceFile(fileName);
    std::string currentLine;

    size_t lineNr = 0;
    while (getline (sourceFile, currentLine)) {
        lineNr++;
        LexLine(currentLine, lineNr);
    }

    SourceLocation sourceLocation {
        .line = ++lineNr,
        .column = 0,
    };
    this->tokens.emplace_back(TokenKind::eof, "", sourceLocation);

    sourceFile.close();
}

void Lexer::LexLine(const std::string& line, size_t lineNr)
{
    std::string current;
    size_t startCol = 0;

    for (size_t i = 0; i < line.size(); i++)
    {
        const auto c = static_cast<char>(line[i]);
        const auto nextChar = i + 1 < line.size() ? line[i + 1] : '\0';
        if (current.empty())
            startCol = i;

        if (!std::isspace(c))
            current.push_back(c);

        if (tokenKindMap.contains(current))
        {
            HandleCompleteToken(current, tokenKindMap[current], lineNr, startCol);
            continue;
        }

        if ((std::isspace(c) || tokenKindMap.contains(std::string {nextChar})) && !current.empty())
        {
            HandleCompleteToken(current, TokenKind::identifier, lineNr, startCol);
            continue;
        }
    }
}

void Lexer::HandleCompleteToken(std::string& tokenText, TokenKind tokenKind, const size_t lineNr, const size_t startCol)
{
    SourceLocation sourceLocation{
        .line = lineNr,
        .column = startCol,
    };

    this->tokens.emplace_back(tokenKind, tokenText, sourceLocation);
    tokenText.clear();
}


