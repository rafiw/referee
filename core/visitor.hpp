#pragma once
#include <memory>
#include <cxxabi.h>
#include <sstream>

template<typename ... Types>
class Visitor
    : public Visitor<Types>...
{
public:
    virtual ~Visitor() = default;
};

template<typename Type>
class Visitor<Type>
    : public virtual Visitor<>
{
public:
    virtual void visit(Type*) = 0;
};


template<typename Base = void, typename Type = void>
class Visitable
    : public Base
{
public:
    using visitable  = void;

    template<typename ... Args>
    Visitable(Args ... args)
        : Base(args ...)
    {
    }

    virtual ~Visitable() = default;

    virtual void accept(Visitor<>& visitor)
    {
        accept_impl(visitor, 0);
    }
    
private:

    template<typename Hack = Base>
    void  accept_impl(Visitor<>& visitor, int)
    {
        Type* self  = dynamic_cast<Type*>(this);
        auto  temp  = dynamic_cast<Visitor<Type>*>(&visitor);
        if(temp)
        {
            temp->visit(self);
        }
        else
        {
            try
            {
                Hack::accept(visitor);
            }
            catch(std::exception& e)
            {
                throw std::runtime_error(std::string(e.what()) + "\n" + error(visitor));
            }
        }
    }

    template<typename Hack = Base>
    typename Hack::visitable  accept_impl(Visitor<>& visitor, float)
    {
        Type* self  = dynamic_cast<Type*>(this);
        auto  temp  = dynamic_cast<Visitor<Type>*>(&visitor);
        if(temp)
        {
            temp->visit(self);
        }
        else
        {
            throw std::runtime_error(error(visitor));
        }
    }

    std::string error(Visitor<>& visitor)
    {
        std::unique_ptr<char, decltype(free)*> vstr(nullptr, &free);
        std::unique_ptr<char, decltype(free)*> self(nullptr, &free);

        vstr.reset(abi::__cxa_demangle(typeid(visitor).name(), 0, 0, nullptr));
        self.reset(abi::__cxa_demangle(typeid(Type).name(), 0, 0, nullptr));

        std::ostringstream  os;
        os  << vstr.get() << " does not implement any visitor for " << self.get();

        return os.str();
    }

};

template<>
class Visitable<void, void>
{
public:
    virtual ~Visitable() = default;
    virtual void  accept(Visitor<>& visitor) {};
};

