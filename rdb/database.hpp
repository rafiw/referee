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

#pragma once

#include <string>
#include <vector>
#include <deque>
#include <sstream>
#include <memory>

/*
class Database
{
public:
    Database();

    void    add_record(uint16_t type, uint64_t timestamp, std::string const& data);
    void    get_record();
};

class   Reader
{
public:
    Reader(Database& database);

    Writer&         get_timestamp(  uint64_t&   data);
    Writer&         get_integer(    int64_t&    data);
    Writer&         get_boolean(    bool&       data);
    Writer&         get_number(     double&     data);
    Writer&         get_string(     std::string&data);
    Writer&         get_size(       unsigned&   data); 

    int64_t         get_integer();
    bool            get_boolean();
    double          get_number();
    std::string     get_string();
    unsigned        get_size(); 

private:
    Database&       m_database;
};
*/

/*
class   Writer
{
public:
    Writer(Database& database);

    TypeBuilder     type();
    void            data(   uint8_t             type,
                            std::string const&  data);

private:
    Writer&         put_timestamp(  uint64_t    data);  // starts a new row
    Writer&         put_integer(    int64_t     data);
    Writer&         put_boolean(    bool        data);
    Writer&         put_number(     double      data);
    Writer&         put_string(     double      data);
    Writer&         put_size(       unsigned    data);

    void            add_type(Type&& type);

private:
    friend class    TypeBuilder;
    
private:
    Database&   m_database;
};
*/

class   Type
{
public:
    virtual ~Type() = default;
};

class   TypeScalar
    : public Type
{
public:
    TypeScalar()    = default;
};

template<typename T>
class   TypeScalarT
    : public TypeScalar
{
public:
    TypeScalarT()   = default;
};

using   TypeString  = TypeScalarT<std::string>;
using   TypeInteger = TypeScalarT<uint64_t>;
using   TypeNumber  = TypeScalarT<double>;
using   TypeBoolean = TypeScalarT<bool>;

class Name2Type
{
public:
    Name2Type(std::string name, Type* type)
        : name(name)
        , type(type)
    {
    }

    std::string name;
    Type* const type;
};

class   TypeRecord
    : public Type
{
public:
    TypeRecord(std::vector<Name2Type>   body)
        : m_body(body)
    {
    }

    std::vector<Name2Type> const&  body() {return m_body;}

private:
    std::vector<Name2Type>  m_body;
};

class   TypeArray
    : public Type
{
public:
    TypeArray(Type* base, unsigned size)
        : base(base)
        , size(size)
    {
    }

    Type*       base;
    unsigned    size;
};   

class   TypeBuilderRecord
{
public:
    TypeBuilderRecord() = default;

    using   Builder     = TypeBuilderRecord;

    Builder&    integer(std::string name);
    Builder&    number( std::string name);
    Builder&    boolean(std::string name);
    Builder&    string( std::string name);
    Builder&    record( std::string name,
                        TypeRecord* type);
    Builder&    array(  std::string name,
                        TypeArray*  type);
    TypeRecord* build();

private:
    std::vector<Name2Type>  m_body;
};

class   TypeBuilderArray
{
public:
    TypeBuilderArray()  = default;

    using   Builder     = TypeBuilderArray;

    Builder&    integer();
    Builder&    number();
    Builder&    boolean();
    Builder&    string();
    Builder&    size(   unsigned    size);
    Builder&    record( TypeRecord* type);
    Builder&    array(  TypeArray*  type);
    TypeArray*  build();

private:
    Type*       m_body  = nullptr;
    unsigned    m_size  = 0;
};

class DataBuilder
{
public:
    DataBuilder();
    DataBuilder(Type* type);
    ~DataBuilder();

    using   Builder     = DataBuilder;

    Builder&    integer(    int64_t             data);
    Builder&    number(     double              data);
    Builder&    boolean(    bool                data);
    Builder&    string(     std::string const&  data);
    Builder&    size(       unsigned            size);
    std::string build();

    class Impl;

private:

    std::unique_ptr<Impl>   m_impl;
};


class Writer
{
public:
    Writer() = default;

    void    open(std::string filename);

    uint8_t decl_type(  Type*               type);

    //  property - constant data
    uint8_t decl_prop(  Type*               type,
                        std::string         name);
    void    push_values(std::string const&  data,
                        uint8_t             prop);
                        
    //  function - time dependent data
    uint8_t decl_func(  Type*               type,
                        std::string         name);

    void    push_values(std::string const&  data,
                        uint64_t            time,
                        uint8_t             func);
};