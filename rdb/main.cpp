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

#include "database.hpp"
#include "utils.hpp"

#include <spdlog/spdlog.h>
#include "spdlog/fmt/fmt.h"
#include <spdlog/fmt/ostr.h>

#include <CLI/App.hpp>
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"

#include <iostream>

using namespace referee::db;

int main(int argc, char* argv[])
{
        CLI::App    app("referee");
    
    std::string refFilename = "default";
    bool        fCsvHeaders = false;
    bool        fLlvmTypes  = false;

    app.add_option( "--read",       refFilename,    "REF file to parse")
        ->check(CLI::ExistingFile);
    app.add_flag(   "--csv-headers",fCsvHeaders,    "Generate CSV headers");
    app.add_flag(   "--llvm-types", fLlvmTypes,     "Dump LLVM types");
    
    try {
        app.parse(argc, argv);

        if(refFilename.empty() == false)
        {
            readDB(refFilename);
        }
    }
    catch (const CLI::ParseError &e)
    {
        return app.exit(e);
    }
    catch(std::exception& e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
        return 1;
    }

#if 0       
        auto    type    = 
            TypeBuilderRecord()
                .integer("i")
                .number("n")
                .string("s")
                .array("xyz", 
                    TypeBuilderArray()
                        .integer()
                        .size(5)
                        .build())
                .record("rec", 
                    TypeBuilderRecord()
                        .boolean("b")
                        .integer("c")
                        .record("what", 
                            TypeBuilderRecord()
                                .integer("X")
                                .build())
                        .build())
                .build();
        print(type);

        std::string     data    = 
            DataWriter(type)
                .integer(0x1234567890abcdef)
                .number(0.5)
                .string("hello")
                .size(5)
                    .integer(1)
                    .integer(2)
                    .integer(3)
                    .integer(4)
                    .integer(5)
                .boolean(true)
                .integer('what')
                    .integer(0x777)
            .build();

        DataReader  reader(data, type);
        int64_t     i;
        double      n;
        bool        b;
        std::string s;
        unsigned    size;

        reader
            .integer(i)
            .number(n)
            .string(s)
            .size(size)
                .integer(i)
                .integer(i)
                .integer(i)
                .integer(i)
                .integer(i)
            .boolean(b)
            .integer(i)
                .integer(i)
            .done();

        printHex(data) << std::endl;

        readData(type, data);

        names(type, "abc");

        readCsv(type, "../test.csv", "abc");
    
        Writer writer;
        writer.open("xyw.rdb");
        auto    typeID  = writer.declType(type);
        auto    funcID  = writer.declProp(typeID, "data");
        writer.pushData(funcID, 1, data);
        writer.close();
#endif
        //readDB("xyw.rdb");

    return 0;
}
