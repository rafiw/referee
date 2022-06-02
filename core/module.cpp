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
    m_name2type["boolean"]  = Factory<TypeBoolean>::create();
    m_name2type["integer"]  = Factory<TypeInteger>::create();
    m_name2type["string"]   = Factory<TypeString>::create();
    m_name2type["number"]   = Factory<TypeNumber>::create();

    m_name2data["__time__"] = Factory<TypeInteger>::create();
}

void    Module::addType(std::string name, Type* type)
{
    if(m_name2type.contains(name))
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    m_name2type[name]   = type;
}

void    Module::addData(std::string name, Type* data)
{
    if(m_name2data.contains(name))
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    m_name2data[name]   = data;
}

void    Module::addConf(std::string name, Type* data)
{
    if(m_name2conf.contains(name))
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    m_name2conf[name]   = data;
}

Type*   Module::getType(std::string name)
{
    if(!m_name2type.contains(name))
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    return m_name2type[name];
}

Type*   Module::getData(std::string name)
{
    if(!m_name2data.contains(name))
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    return m_name2data[name];
}

Type*   Module::getConf(std::string name)
{
    if(!m_name2conf.contains(name))
    {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    return m_name2conf[name];
}

bool    Module::hasType(std::string name)
{
    return m_name2type.contains(name);
}

bool    Module::hasData(std::string name)
{
    return m_name2data.contains(name);
}

bool    Module::hasConf(std::string name)
{
    return m_name2conf.contains(name);
}

std::vector<std::string>    Module::getTypeNames()
{
    std::vector<std::string>    names;

    for(auto iter: m_name2type)
    {
        names.push_back(iter.first);
    }

    return names;
}


void    Module::pushContext(std::string name)
{
    m_context.push_back(name);
}

void    Module::popContext()
{
    m_context.pop_back();
}

bool    Module::hasContext(std::string name)
{
    return std::find(m_context.begin(), m_context.end(), name) != m_context.end();
}