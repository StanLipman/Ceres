cmake_minimum_required(VERSION 4.3)
project(Ceres)

set(CMAKE_CXX_STANDARD 26)

include_directories(.)

add_executable(Ceres
        Main.cpp
        FileIO/WriteDllFile.h
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
        Models/Types/Type.h
        Models/Symbols/Symbol.h
        Models/Scopes/Scope.h
        Passes/DeclarationCollector.h
        Passes/DeclarationResolver.h
        Passes/SemanticAnalyzer.h
        Models/CompilerContext.h
        Models/Symbols/ConstellationSymbol.h
        Models/Symbols/MethodSymbol.h
        Models/Ast/TransmitAstNode.cpp
        Models/Ast/TransmitAstNode.h
        Models/Ast/LiteralValueAstNode.cpp
        Models/Ast/LiteralValueAstNode.h
        Parser/LiteralValueParser.cpp
        Parser/LiteralValueParser.h
        Passes/TypeChecker.h
        Parser/TransmitParser.cpp
        Parser/TransmitParser.h
        Models/Ast/VariableAstNode.cpp
        Models/Ast/VariableAstNode.h
        "MetaData/PE Headers/MsDosHeader.h"
        "MetaData/PE Headers/FileHeader.h"
        "MetaData/PE Headers/OptionalHeader.h"
        MetaData/CreateMetaData.h
)
