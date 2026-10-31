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
        Models/TokenKind.h
        Models/Ast/ArgumentAstNode.h
        Semantics/SemanticModel.h
        Semantics/Scope.h
        Semantics/Symbol.h
        TypeSystem/Type.h
        Passes/AstVisitor.h
        Models/Ast/DocumentAstNode.cpp
        Passes/PrintAstPass.h
        Models/Ast/GalaxyAstNode.cpp
        Models/Ast/TypeAstNode.cpp
        Models/Ast/ArgumentAstNode.cpp
        Models/Ast/BodyAstNode.cpp
        Models/Ast/ConstellationAstNode.cpp
        Models/Ast/MethodAstNode.cpp
        Passes/PrintAstPass.cpp
)
