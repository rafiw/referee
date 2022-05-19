/*
 *  MIT License
 *  
 *  Copyright (c) 2022 Michael Rolnik
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */


#include "module.hpp"
#include "factory.hpp"

Module::Module(std::string name)
{
    m_name2data["boolean"]  = Factory<TypeBoolean>::create();
    m_name2data["integer"]  = Factory<TypeInteger>::create();
    m_name2data["string"]   = Factory<TypeString>::create();
    m_name2data["number"]   = Factory<TypeNumber>::create();
}

void    Module::add_type(std::string name, Type* type)
{
    if(m_name2type.contains(name))
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    m_name2type[name]   = type;
}

void    Module::add_data(std::string name, Type* data)
{
    if(m_name2data.contains(name))
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    m_name2data[name]   = data;
}

Type*   Module::get_type(std::string name)
{
    if(!m_name2type.contains(name))
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    return m_name2type[name];
}

Type*   Module::get_data(std::string name)
{
    if(!m_name2data.contains(name))
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    return m_name2data[name];
}

void    Module::push_context(std::string name)
{
    m_context.push_back(name);
}

void    Module::pop_context()
{
    m_context.pop_back();
}

bool    Module::has_context(std::string name)
{
    return std::find(m_context.begin(), m_context.end(), name) != m_context.end();
}