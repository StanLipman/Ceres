cmake_minimum_required(VERSION 4.3)
project(Ceres)

set(CMAKE_CXX_STANDARD 26)

include_directories(.)

add_executable(Ceres
        Main.cpp
        Lexer/Lexer.cpp
        Lexer/Lexer.h
        Models/Token.h
        Parser/ParserManager.h
        Models/Ast/AstNodes.h
        Models/Ast/DocumentAstNode.h
        Models/Ast/ConstellationAstNode.h
        Models/Ast/MethodAstNode.h
        Models/Ast/BodyAstNode.h
        Models/Ast/GalaxyAstNode.h
        Models/Ast/TypeAstNode.h
        Parser/GalaxyParser.h
        Parser/DocumentParser.h
        Parser/BaseParser.h
        Parser/GalaxyParser.cpp
        Parser/DocumentParser.cpp
        Parser/BodyParser.cpp
        Parser/BodyParser.h
        Parser/ConstellationParser.cpp
        Parser/ConstellationParser.h
        Parser/MethodParser.cpp
        Parser/MethodParser.h
)
