#ifndef __DSCRIPT_COMPILER_H__
#define __DSCRIPT_COMPILER_H__

////////////////////////////////////////////////////////////////////////////////
// DScript Scripting Language
// Copyright (C) 2003 Bryan "daerid" Ross
//
// Permission to copy, use, modify, sell and distribute this software is
// granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.
////////////////////////////////////////////////////////////////////////////////

/*<<<<<COPYLEFT*/
/** @file rtkcompiler.h Compiler of the rtk script grammar
 * RTK The report toolkit. Copyright (C) 2003-2013 Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
////////////////////////////////////////////////////////////////////////////////
// Standard Library Include Files
#include <string>
#include <stdexcept>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// DScript Include Files
#include "rtkinstruction.h"
#include "rtkstringtable.h"
#include "rtkfloattable.h"
////////////////////////////////////////////////////////////////////////////////

namespace dscript
{
/// Compile a string of dscript code into a codeblock
codeblock_t compile(
    const std::string& code,
    string_table& strings,
    float_table& floats
);

/// Saves a compiled codeblock to a binary file, for faster loading times
void save_codeblock(
    const std::string& filename,
    const codeblock_t& code
);

/// Loads a compiled file saved with save_codeblock() into a codeblock,
/// which can be executed by the vmachine
codeblock_t load_compiled_file(
    const std::string& filename,
    string_table& strings,
    float_table& floats
);

/// This is a support structure for tracking parse and compile
/// errors, using exceptions.
struct code_position
{
    code_position(int l,int c) : line(l), col(c) {}
    code_position(const code_position& other)
        : line(other.line), col(other.col)
    {}
    code_position& operator = (const code_position& other)
    {
        line = other.line;
        col = other.col;
        return *this;
    }
    int line;
    int col;
};

/// This is thrown when the compiler has encountered a semantic error
/// This is not touched by the actual parser
struct compiler_error : public std::runtime_error
{
    code_position pos;
    compiler_error(const std::string& msg,const code_position& p)
        : runtime_error(msg), pos(p)
    {}
};
}

#endif//__DSCRIPT_COMPILER_H__
