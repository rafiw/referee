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

//#include "database.hpp"

//void    Writer::data(uint8_t type, std::string const& data)
//{
    
//}
#include <stdint.h>
#include <string>
#include <fstream>
#include <chrono>
#include <iostream>

struct RecordInfo 
{
    uint64_t    timestamp;
    uint32_t    size;    
    uint8_t     type;
    uint8_t     subtype;
};

class   Database
{
public:
    Database();

    void    open(std::string filename);
    void    close();

    void    write(uint64_t timestamp, uint16_t type, uint16_t subtype, std::string data);

    bool    valid();
    uint64_t    timestamp() {return m_timestamp;}
    uint16_t    type()      {return m_type;}
    uint16_t    subtype()   {return m_subtype;}
    std::string data();

    void    next();
    void    reset();

private:
    std::fstream    m_file;
    uint64_t        m_size      = 0;
    uint64_t        m_timestamp;
    uint16_t        m_type;
    uint16_t        m_subtype;
    std::string     m_data;
    bool            m_has_data  = false;

};

Database::Database()
{

}

void    Database::open(std::string filename)
{
    m_file.open(filename, std::ios_base::binary | std::ios_base::in);
    next();
}

void    Database::close()
{
    m_file.flush();
    m_file.close();
}


void    Database::write(uint64_t timestamp, uint16_t type, uint16_t subtype, std::string data)
{
    uint64_t    size    = data.length();
    m_file.write(reinterpret_cast<char const*>(&size), sizeof(size));
    m_file.write(reinterpret_cast<char const*>(&type), sizeof(type));
    m_file.write(reinterpret_cast<char const*>(&subtype), sizeof(subtype));
    m_file.write(reinterpret_cast<char const*>(&timestamp), sizeof(timestamp));
    m_file.write(reinterpret_cast<char const*>(data.data()), data.length());
}

uint64_t    timestamp()
{
    return  std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


bool    Database::valid()
{
    return m_file.good();
}

void    Database::next()
{
    uint64_t    size;

    if(m_has_data == false)
    {
        m_file.seekg(m_size, std::ios_base::cur);
    }

    m_file.read(reinterpret_cast<char*>(&m_size), sizeof(m_size));
    m_file.read(reinterpret_cast<char*>(&m_type), sizeof(m_type));
    m_file.read(reinterpret_cast<char*>(&m_subtype), sizeof(m_subtype));
    m_file.read(reinterpret_cast<char*>(&m_timestamp), sizeof(m_timestamp));
    m_has_data = false;
}

std::string Database::data()
{
    if(m_has_data)
        return m_data;

    m_data.resize(m_size);
    m_file.read(reinterpret_cast<char*>(m_data.data()), m_size);
    m_has_data = true;

    return m_data;
}


void    Database::reset()
{
    m_file.seekg(0, std::ios_base::beg);
}

int main(int argc, char* argv[])
{
    Database    db;
#if 0
    db.open("./database.rdb");

    db.write(timestamp(), 0, 0, "referee");
    db.write(timestamp(), 0, 1, "1.0.0");
    db.write(timestamp(), 1, 2, "hello");
    db.write(timestamp(), 1, 2, "we are here");
    db.write(timestamp(), 1, 2, "why");

    db.close();
#else
    db.open("./database.rdb");

    while(db.valid())
    {
        std::cout << db.timestamp() << " " << db.data() << std::endl;
        db.next();
    }
#endif

}