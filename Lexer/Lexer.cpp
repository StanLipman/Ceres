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

void Lexer::LexLine(const std::string& line, const size_t lineNr)
{
    std::string current;
    size_t startCol = 0;

    for (size_t i = 0; i < line.size(); i++)
    {
        const char c = line[i];
        const auto nextChar = i + 1 < line.size() ? line[i + 1] : '\0';
        if (current.empty())
            startCol = i;

        if (!std::isspace(c) && c != '\"' && c != '\'')
            current.push_back(c);

        if (c == '\"')
        {
            while (true)
            {
                i++;
                const char stringChar = line[i];
                if (stringChar == '\"')
                    break;

                current.push_back(stringChar);
            }

            HandleCompleteToken(current, TokenKind::_string, lineNr, startCol);

            continue;
        }

        if (c == '\'')
        {
            while (true)
            {
                i++;
                const char charChar = line[i];
                if (charChar == '\'')
                    break;

                current.push_back(charChar);
            }

            HandleCompleteToken(current, TokenKind::_char, lineNr, startCol);

            continue;
        }

        if (std::isdigit(c) && current.size() == 1) // size == 1 because we add the first digit at the start
        {
            bool isFloat = false;
            while (true)
            {
                i++;
                const char digitChar = line[i];
                if (digitChar == '.')
                    isFloat = true;

                if (!std::isdigit(digitChar) && digitChar != '.')
                {
                    i--; // Set the iterator one back to make sure that the non numeric value still gets read
                    break;
                }

                current.push_back(digitChar);
            }

            if (isFloat)
                HandleCompleteToken(current, TokenKind::_float, lineNr, startCol);
            else
                HandleCompleteToken(current, TokenKind::_int, lineNr, startCol);

            continue;
        }

        if (current == "true" || current == "false")
        {
            HandleCompleteToken(current, TokenKind::_bool, lineNr, startCol);
            continue;
        }

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

void Lexer::LexNumericValue(const std::string& line, size_t lineNr)
{
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


