#pragma once
#include "Symbol.h"

class MethodSymbol : public Symbol
{
public:
    std::shared_ptr<MethodType> type;
    const MethodAstNode* declaration;
};

