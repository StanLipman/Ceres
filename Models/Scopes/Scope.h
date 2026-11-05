#pragma once
#include <memory>
#include <unordered_map>

#include "Models/Symbols/Symbol.h"

class Scope
{
public:
    enum ScopeTypes
    {
        global,
        document,
        galaxy,
        constellation,
        method,
    };

private:
    ScopeTypes scopeType;
    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols;

public:
    std::shared_ptr<Scope> parent = nullptr;

    explicit Scope(const ScopeTypes type, const std::shared_ptr<Scope>& parent = nullptr) : scopeType(type), parent(parent) {};

    std::shared_ptr<Symbol> Lookup(const std::string& name)
    {
        auto it = symbols.find(name);

        if (it != symbols.end())
            return it->second;

        if (parent)
            return parent->Lookup(name);

        return nullptr;
    }

    bool Insert(std::shared_ptr<Symbol> symbol)
    {
        return symbols.emplace(symbol->name, std::move(symbol)).second;
    }
};
