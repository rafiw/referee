#pragma once
#include "visitor.hpp"
#include "syntax.hpp"

class Negated
{
public:
    static Expr* make(Expr* expr);
};
