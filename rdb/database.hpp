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

class   Type
{
public:
};

class   TypeBuilder
{
public:
    TypeBuilder&    add_timestamp(  std::string const&  name);
    TypeBuilder&    add_integer(    std::string const&  name);
    TypeBuilder&    add_boolean(    std::string const&  name);
    TypeBuilder&    add_number(     std::string const&  name);
    TypeBuilder&    add_string(     std::string const&  name);
    TypeBuilder&    enter_array(    std::string const&  name);
    TypeBuilder&    enter_array(    std::string const&  name,
                                    unsigned            size);
    TypeBuilder&    leave_array();
    TypeBuilder&    enter_sruct(    std::string const&  name);
    TypeBuilder&    leave_struct();
    
    Type*           build();
};

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