#pragma once
#include "visitor.hpp"
#include "syntax.hpp"

class Negated
{
public:
    Negated();
    virtual ~Negated();
    
    static Expression* make(Expression* expr);
    
private:
    struct Details;
    std::unique_ptr<Details> details;
};
