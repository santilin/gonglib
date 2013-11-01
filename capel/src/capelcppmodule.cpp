
#include "gonglib_inc.h"
#include <capelcppmodule.h>


using namespace gong;

namespace capel {

const char *gpllicense =
" *\n"
" * This program is free software; you can redistribute it and/or modify\n"
" * it under the terms of the GNU General Public License as published by\n"
" * the Free Software Foundation; either version 2 of the License, or\n"
" * (at your option) any later version.\n"
" *\n"
" * This program is distributed in the hope that it will be useful,\n"
" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
" * GNU General Public License for more details.\n";

const char *lgpllicense =
" *\n"
" * This library is free software; you can redistribute it and/or\n"
" * modify it under the terms of the GNU General Public\n"
" * License as published by the Free Software Foundation; either\n"
" * version 2 of the License, or (at your option) any later version.\n"
" *\n"
" * This library is distributed in the hope that it will be useful,\n"
" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU\n"
" * General Public License for more details.\n"
" *\n"
" * You should have received a copy of the GNU General Public\n"
" * License along with this library. If not, see <http://www.gnu.org/licenses/>.\n";

const char *shortlgpllicense =
" *\n"
" * This library is free software; you can redistribute it and/or\n"
" * modify it under the terms of the GNU General Public\n"
" * License as published by the Free Software Foundation; either\n"
" * version 2 of the License, or (at your option) any later version.\n"
" *\n"
" * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.\n";

Xtring CppModule::plural(const Xtring &singular)
{
	Xtring slower = singular.lower();
	switch( slower.last(1)[0] ) {
	case 'a':
	case 'e':
	case 'i':
	case 'o':
	case 'u':
		return singular + "s";
	default:
		return singular + "es";
	}
}

Xtring CppModule::methodGetName(const Xtring &type, const Xtring &membername)
{
	if( type == "bool" ) {
		if( membername.left(3).lower() == "can" )
			return membername.left(1).lower() + membername.mid(1);
		else
			return "is" + membername;
	} else
		return "get" + membername;
}


Xtring CppModule::unpackType(const Xtring &type)
{
	if( type.right( 1 ) == "*" ) // puntero
		return type.left( type.length() - 1 ) + "*";
	else
		return type;
}

Xtring CppModule::memberName(const Xtring &type, const Xtring &membername)
{
	if( type.right( 1 ) == "*" ) // puntero
		return "p" + membername;
	else
		return "m" + membername;
}

Xtring CppModule::functionParameter(const Xtring &type)
{
	if( type == "int" || type == "char" || type == "double" || type == "float" || type == "uint" || type == "bool" )
		return type;
	else if( type.right( 1 ) == "*" ) // puntero
		return type;
	else
		return "const " + type + "&";
}

Xtring CppModule::functionReturn(const Xtring & type, bool isreturnconst)
{
	if( type == "int" || type == "char" || type == "double" || type == "float" || type == "uint" || type == "bool" )
		return type;
	else if( type.right( 1 ) == "*" ) // puntero
		return type;
	else if( isreturnconst )
		return "const " + type + "&";
	else return type;
}


Xtring CppModule::extractClassname(const Xtring &fullclassname)
{
	if( fullclassname.find("::") != Xtring::npos ) {
		return fullclassname.mid(fullclassname.find("::") + 2);
	} else {
		return fullclassname;
	}
}

Xtring CppModule::extractNamespace(const Xtring &fullclassname)
{
	if( fullclassname.find("::") != Xtring::npos ) {
		return fullclassname.left( fullclassname.find("::") );
	} else {
		return Xtring();
	}
}

Xtring CppModule::getDefault(const Xtring &type, const Xtring &defaultvalue)
{
	if( defaultvalue == "0" ) {
		if( type == "int" )
			return "0";
		else if( type == "uint" )
			return "0";
		else if( type == "bool" )
			return "false";
		else if( type.right( 1 ) == "*" ) // puntero
			return "0";
		else if( type == "Xtring" )
			return "Xtring::null";
		else
			return type + "()";
	} else if( type == Xtring() ) {
		return "\"" + defaultvalue + "\"";
	} else
		return defaultvalue;
}


void CppModule::addCopyLeft(const Xtring &project, const Xtring &desc,
	const Xtring &license)
{
	Xtring text;
	text += "/** ";
	text += "@file " + gong::FileUtils::filename(mFilename) + " " + desc + "\n";
	text += " * " + project + "\n";
	if( license.length() == 0 )
		text += lgpllicense;
	else
		text += license;
	text += " */\n";
	insert_extrusion_at(0, "COPYLEFT", text);
}

void CppModule::addHeaderPreamble(const Xtring &fullclassname)
{
	Xtring classname = extractClassname(fullclassname);
	Xtring _namespace = extractNamespace(fullclassname);

	insert_extrusion_at(0, classname.upper() + "_PREAMBLE",
		"#ifndef _" + getHeaderProtect(fullclassname) + "\n"
		"#define _" + getHeaderProtect(fullclassname) + "\n", "\n");
}

void CppModule::addHeaderPostamble(const Xtring &fullclassname)
{
	Xtring classname = extractClassname(fullclassname);
	Xtring _namespace = extractNamespace(fullclassname);

	Xtring postamble;
	if( !_namespace.isEmpty() )
		postamble += "} // namespace " + _namespace + "\n";
	postamble += "} // namespace gong\n";
	postamble += "#endif // " + getHeaderProtect(fullclassname);

	insert_extrusion(classname.upper() + "_POSTAMBLE", postamble, "\n");
}


Xtring CppModule::getHeaderProtect(const Xtring &fullclassname) const
{
	Xtring _namespace = extractNamespace(fullclassname);
	return Xtring((_namespace.empty()?"_":_namespace) + "_" + FileUtils::filename(Xtring(getFilename()).replace(".", "_"))).upper();
}

void CppModule::addHeaderProtect(const Xtring &fullclassname)
{
	addHeaderPreamble( fullclassname );
	addHeaderPostamble( fullclassname );
}

void CppModule::addClassDefinition(const Xtring &fullclassname,
	const XtringList &members, const Xtring &classprops)
{
	Xtring classname = extractClassname( fullclassname );
	Xtring _namespace = extractNamespace( fullclassname );
	bool adddefaultconstr = (classprops.find("defconstr") != Xtring::npos );
	bool addconstr = (classprops.find("fullconstr") != Xtring::npos );
	bool set_returns_reference = (classprops.find("set_returns_reference") != Xtring::npos );

	addHeaderProtect( fullclassname );

	Xtring get, set, full_constructor, full_constructor_init_members, memb;

	for( XtringList::const_iterator it = members.begin();
		it != members.end();
		++ it ) {
		XtringList memberinfo;
		(*it).tokenize( memberinfo, " " );
		Xtring membername = memberinfo[0];
		Xtring type = unpackType((memberinfo.size()>1?memberinfo[1]:""));
		Xtring props = memberinfo.size()>2?memberinfo[2]:"";
		Xtring defaultvalue = memberinfo.size()>3?memberinfo[3]:"";
		if( defaultvalue == "0" )
			defaultvalue = getDefault(type, defaultvalue);
		bool isread = (props.find("r") != Xtring::npos);
		bool iswrite = (props.find("w") != Xtring::npos);
		bool isparameterconstr = (props.find("c") != Xtring::npos);
		if( isparameterconstr ) {
			full_constructor += ", " + functionParameter(type) + " " + membername.lower();
			if( !defaultvalue.isEmpty() ) {
				full_constructor += " = " + defaultvalue;
			}
			full_constructor_init_members += ", " + memberName( type, membername) + "(" + membername.lower() + ")";
		}
		if( isread )
			get += "\t" + functionParameter(type) + " " + methodGetName(type, membername)
				+ "() const { return " + memberName(type, membername) + "; }\n";
		if( iswrite ) {
			if( set_returns_reference )
				set += "\t" + classname + "* set";
			else
				set += "\tvoid set";
			set += membername + "(" + functionParameter(type) + " "
				+ membername.lower() + ") { " + memberName(type, membername)
				+ " = " + membername.lower() + ";";
			if( set_returns_reference )
				set += " return this; }\n";
			else
				set +=" }\n";
		}
		memb += "\t" + type + " " + memberName(type, membername) + ";\n";
		if( it == members.end() )
			break;
	}

	Xtring predefinition;
	if( !_namespace.isEmpty() )
		predefinition += "namespace " + _namespace + " {\n\n";
	predefinition +=
		"class " + classname + "\n"
		"{\n";
	Xtring definition =	"public:\n";
	if( adddefaultconstr ) {
		definition +=
			"\t " + classname + "();\n"
			"\t~" + classname + "();\n\n";
	}
	definition += get + "\n" + set;
	definition +=
		"private:\n" + memb;

	insert_extrusion(classname.upper() + "_CLASS", definition, "};\n", predefinition);

	if( full_constructor.length()>2 && addconstr ) {
		full_constructor = full_constructor.mid(2);
		full_constructor_init_members = full_constructor_init_members.mid(2);
		insert_extrusion(classname.upper() + "_FULLCONSTRUCTOR",
			"\t" + classname + "(" + full_constructor + ")\n"
			"\t\t: " + full_constructor_init_members,
			"\n{\n}\n");
	}
}

}


