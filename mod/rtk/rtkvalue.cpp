////////////////////////////////////////////////////////////////////////////////
// DScript Scripting Language
// Copyright (C) 2003 Bryan "daerid" Ross
//
// Permission to copy, use, modify, sell and distribute this software is
// granted provided this copyright notice appears in all copies. This
// software is provided "as is" without express or implied warranty, and
// with no claim as to its suitability for any purpose.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Standard Library Includes
#include <sstream>
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// DScript Includes
#include "rtkvalue.h"
#include <gongvariant.h>
#include <gongxtring.h>
////////////////////////////////////////////////////////////////////////////////

using namespace std;
using namespace dscript;
using gong::DateTime;
using gong::Date;
using gong::Time;

value::value()
    : type(type_str), strval(""), intval(0), fltval(0), dateval(Date(0),Time(0))
{
}

value::value(int i)
    : type(type_int), intval(i)
{
}

value::value(double d)
    : type(type_flt), fltval(d)
{
}

value::value(const DateTime dt)
    : type(type_date), dateval(dt)
{
}

value::value(const std::string& s)
    : type(type_str), strval(s)
{
}

value::value(string_table::entry s)
    : type(type_str), strval(s)
{
}

value::value(const string& s, bool )
    : type(type_binary), strval(s)
{
}


value::value(const value& other)
    : type(other.type),
      strval(other.strval),
      intval(other.intval),
      fltval(other.fltval),
      dateval(other.dateval)
{
}
/// SCT
value::value(const gong::Variant& v)
{
	switch( v.type() ) {
		case gong::Variant::tInt:
		case gong::Variant::tLong:
		case gong::Variant::tBool:
			type = type_int;
			intval = v.toInt();
			break;
		case gong::Variant::tDouble:
		case gong::Variant::tMoney:
			type = type_flt;
			fltval = v.toDouble();
			break;
		case gong::Variant::tBinary:
			type = type_binary;
			strval = v.toString();
			break;
		case gong::Variant::tDate:
		case gong::Variant::tDateTime:
		case gong::Variant::tTime:
			type = type_date;
			dateval = v.toDate();
			break;
		case gong::Variant::tString:
		default:
			type = type_str;
			strval = v.toString();
			break;
	}
}

gong::Variant value::valueToVariant(const value& v)
{
    switch(v.type)
    {
		case type_int:
			return gong::Variant( v.intval );
		case type_flt:
			return gong::Variant( v.fltval );
		case type_date:
			return gong::Variant( v.dateval );
		case type_binary:
			return gong::Variant( static_cast<const void *>(v.strval.c_str()), v.strval.size() );
		default:
			return gong::Variant( v.strval );
    }
}

/// SCT

value& value::operator = (const std::string& s)
{
    type = type_str;
    strval = s;
    return *this;
}

value& value::operator = (string_table::entry s)
{
    type = type_str;
    strval = s;
    return *this;
}

value& value::operator=(const gong::DateTime dt)
{
    type = type_date;
    dateval = dt;
    return *this;
}

value& value::operator = (int i)
{
    type = type_int;
    intval = i;
    return *this;
}

value& value::operator = (double d)
{
    type = type_flt;
    fltval = d;
    return *this;
}

value& value::operator =(const value& other)
{
    type = other.type;
    intval = other.intval;
    strval = other.strval;
    fltval = other.fltval;
    dateval = other.dateval;
    return *this;
}

string value::to_str() const
{
    switch(type)
    {
    case type_int:
    {
        stringstream s;
        s << intval;
        return s.str();
    }
    break;
    case type_flt:
    {
        stringstream s;
        s << fltval;
        return s.str();
    }
    break;
    case type_date:
        return dateval.toString();
    default:
        return strval;
    }
}

int value::to_int() const
{
    switch(type)
    {
    case type_str:
    case type_binary:
    {
        int i = 0;
        stringstream s;
        s << strval;
        s >> i;
        return i;
    }
    break;
    case type_flt:
        return (int)fltval;
        break;
    default:
        return intval;
    }
}

double value::to_flt() const
{
    switch(type)
    {
		case type_str:
		case type_binary:
		{
			bool ok = true;
			double d = static_cast<gong::Xtring>(strval).toDouble(&ok);
			return d;
		}
		break;
		case type_int:
			return (double)intval;
			break;
		default:
			return fltval;
    }
}

DateTime value::to_datetime() const
{
    switch(type)
    {
    case type_str:
    case type_binary:
        return DateTime( strval );
        break;
    case type_int:
        return DateTime( Date(intval), Time(0));
        break;
    case type_flt:
        return DateTime( Date(int(fltval)), Time( fltval-int(fltval))  );
        break;
    default:
        return dateval;
    }
}

void value::set_type(value::ty new_type)
{
    switch(new_type)
    {
    case type_str:
    case type_binary:
        strval = to_str();
        break;
    case type_int:
        intval = to_int();
        break;
    case type_flt:
        fltval = to_flt();
        break;
    case type_date:
        dateval = to_datetime();
        break;
    }
    type = new_type;
}
