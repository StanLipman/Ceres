#pragma once
#include "Symbol.h"
#include "Models/Ast/ConstellationAstNode.h"

class ConstellationSymbol : public Symbol
{
public:
    std::shared_ptr<ConstellationType> type;
    const ConstellationAstNode* declaration;
};
