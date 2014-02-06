#include <gongdebug.h>
#include "capelphpmodule.h"

using namespace gong;

namespace capel {

Xtring PhpModule::getPHPTypeFromMysqlType(SqlColumnType mysqltype)
{
	_GONG_DEBUG_PRINT( 0, mysqltype );
	switch( mysqltype ) {
		case SQLSTRING:
		case SQLTEXT:
		case SQLBLOB:
			return "string";
		case SQLINTEGER:
			return "integer";
		case SQLDATE:
		case SQLDATETIME:
		case SQLTIME:
		case SQLTIMESTAMP:
			return "integer";
		case SQLBOOL:
			return "bool";
		case SQLDECIMAL:
		case SQLFLOAT:
			return "float";
	}
}
	
	
	
}