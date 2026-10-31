#pragma once
#include <unordered_map>

#include "Symbol.h"

class Scope {
private:
    Scope* parent;

    std::unordered_map<std::string, Symbol> symbols;

public:
    explicit Scope(Scope* parent = nullptr)
        : parent(parent) {}

    bool declare(Symbol symbol) {
        return symbols.emplace(
            symbol.name,
            std::move(symbol)
        ).second;
    }

    Symbol* lookup(const std::string& name) {
        const auto it = symbols.find(name);

        if (it != symbols.end())
            return &it->second;

        if (parent)
            return parent->lookup(name);

        return nullptr;
    }
};
