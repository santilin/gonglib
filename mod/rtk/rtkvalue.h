////////////////////////////////////////////////////////////////////////////////
// DScript Scripting Language
// Copyright (C) 2003 Bryan "daerid" Ross
//
// Permission to copy, use, modify, sell and distribute this software is
// granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.
////////////////////////////////////////////////////////////////////////////////

#ifndef __DSCRIPT_VALUE_H__
#define __DSCRIPT_VALUE_H__

////////////////////////////////////////////////////////////////////////////////
// Standard Library Includes
#include <string>
#include <map>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// DScript Includes
#include "rtkinstruction.h"
////////////////////////////////////////////////////////////////////////////////

#include <gongdate.h>
#include <gongvariant.h>
/*
	ChangeLog
	2008/12/02  Added date type
	/// \todo Add datetime, time and money types
*/

namespace dscript
{
/// This is the actual value data type that DScript works with.
struct value
{
    enum ty
    {
        type_str,
        type_int,
        type_flt,
        type_date,
        type_binary
    };
    ty type;

    value();
    value(int i);
    value(double d);
    value(const gong::DateTime dt);
    value(const std::string& s);
    value(const std::string& s, bool); // binary
    value(string_table::entry s);
/// SCT
	value(const gong::Variant &v);
	static gong::Variant valueToVariant(const value &v);
/// SCT
    value(const value& other);
    value& operator = (const value& other);

    value& operator = (const std::string& s);
    value& operator = (string_table::entry s);
    value& operator = (int i);
    value& operator = (double d);
    value& operator = (const gong::DateTime dt);

    std::string to_str() const;
    int to_int() const;
    double to_flt() const;
    gong::DateTime to_datetime() const;

    void set_type(ty new_type);
    void clear() {
        strval = "";
        intval = 0;
        fltval = 0.0;
        type = type_str;
    }

    std::string strval;
    int intval;
    double fltval;
    gong::DateTime dateval;
};

typedef std::map<string_table::entry,value,cmp_ste> dictionary_t;
}

inline std::ostream& operator << (std::ostream& out, const dscript::value& v)
{
    switch(v.type)
    {
    case dscript::value::type_str:
    case dscript::value::type_binary:
        out << v.strval;
        break;
    case dscript::value::type_int:
        out << v.intval;
        break;
    case dscript::value::type_flt:
        out << v.fltval;
        break;
    case dscript::value::type_date:
        out << v.dateval.toString();
        break;
    }
    return out;
}

#endif//__DSCRIPT_VALUE_H__
