#include <gongdebug.h>
#include <ctype.h>
#include "capelphpmodule.h"

using namespace gong;

namespace capel {

Xtring PhpModule::getPHPTypeFromMysqlType(SqlColumnType mysqltype)
{
	switch( mysqltype ) {
		case SQLSTRING:
		case SQLTEXT:
		case SQLBLOB:
			return "string";
		case SQLINTEGER:
			return "integer";
		case SQLDATE:
		case SQLTIME:
			return "date";
		case SQLDATETIME:
		case SQLTIMESTAMP:
			return "datetime";
		case SQLBOOL:
			return "bool";
		case SQLDECIMAL:
		case SQLFLOAT:
			return "float";
	}
}
	
/**
	* @brief converts an identifer like identifier_word to a model name like IdentifierWord
	* 
	* @param identifier ...
	* @return Xtring
	*/
Xtring PhpModule::modelize(const Xtring& identifier)
{
	Xtring ret;
	XtringList parts;
	identifier.tokenize( parts, "_" );
	for( XtringList::const_iterator it = parts.begin(); it != parts.end(); ++it ) {
		ret += it->proper();
	}
	return ret;
}


Xtring PhpModule::unModelize(const Xtring &identifier, const Xtring &separator)
{
	Xtring ret;
	for( auto &ch: identifier) {
 		if( ::isupper(ch) ) {
			if (ret.size()) {
				ret += separator;
			}
			ret += ::tolower(ch);
		} else {
			ret += ch;
		}
	}
	return ret;
}

Xtring PhpModule::toIdentifier(const Xtring& identifier)
{
	Xtring ret;
	for( auto &ch: identifier) {
 		if( !isalnum(ch) ) {
			ret += '_';
		} else {
			ret += ch;
		}
	}
	return ret;
}



}
