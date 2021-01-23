#pragma once
#include "visitor.hpp"
#include "syntax.hpp"

class Canonic
{
public:
    Canonic();
    virtual ~Canonic();
    
    static Expression* make(Expression* expr);

private:
    
private:
    struct Details;
    std::unique_ptr<Details> details;
};
