#pragma once
#include "visitor.hpp"
#include "syntax.hpp"

class Canonic
{
public:
    static Expr*    make(Expr* expr);
};
