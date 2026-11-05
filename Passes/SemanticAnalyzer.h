#pragma once
#include <memory>

#include "DeclarationCollector.h"
#include "DeclarationResolver.h"
#include "Models/Scopes/Scope.h"

class SemanticAnalyzer
{
private:
    std::shared_ptr<Scope> globalScope = std::make_shared<Scope>(Scope::ScopeTypes::global);
    CompilerContext compilerContext;

public:
    SemanticAnalyzer()
    {
        CreatePrimitiveTypes();
    }

    void analyze(const std::shared_ptr<DocumentAstNode>& document) const
    {
        // The idea here is:
        // 1. We collect every type that there is, but we do not yet process it
        //    Types in this case are: constellations and methods of constellations
        //    When we do this, we know what we have, but not what it means.
        // 2. We resolve the types that we have collected in step 1.
        //    We go back over every ast node, and everywhere where we can find a reference to where a type is used, we check if one had been registered.

        DeclarationCollector declarationCollector(globalScope, compilerContext);
        declarationCollector.visit(*document);

        DeclarationResolver declarationResolver(globalScope);
        declarationResolver.visit(*document);
    }

private:
    void CreatePrimitiveTypes()
    {
        MakePrimitiveType("vacuum", Type::TypeKind::vacuum);
        MakePrimitiveType("bool", Type::TypeKind::_bool);
        MakePrimitiveType("byte", Type::TypeKind::_byte);
        MakePrimitiveType("char", Type::TypeKind::_char);
        MakePrimitiveType("decimal", Type::TypeKind::_decimal);
        MakePrimitiveType("double", Type::TypeKind::_double);
        MakePrimitiveType("float", Type::TypeKind::_float);
        MakePrimitiveType("short", Type::TypeKind::_short);
        MakePrimitiveType("ushort", Type::TypeKind::_ushort);
        MakePrimitiveType("int", Type::TypeKind::_int);
        MakePrimitiveType("uint", Type::TypeKind::_uint);
        MakePrimitiveType("long", Type::TypeKind::_long);
        MakePrimitiveType("ulong", Type::TypeKind::_ulong);
        MakePrimitiveType("string", Type::TypeKind::string);
        }

    void MakePrimitiveType(const std::string& name, const Type::TypeKind kind)
    {
        const auto symbol = std::make_shared<ConstellationSymbol>();
        symbol->name = name;
        symbol->symbolId = compilerContext.GetNextSymbolId();
        symbol->declaration = nullptr; // We do not have this

        const auto type = std::make_shared<ConstellationType>();
        type->declarationSymbolId = symbol->symbolId;
        type->id = compilerContext.GetNextTypeId();
        type->kind = kind;

        symbol->type = type;

        const auto result = globalScope->Insert(symbol);
        assert(result); // Else things have gone very wrong
    }
};
