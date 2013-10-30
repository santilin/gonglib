#include <gongdebug.h>
#include <gongcsvutils.h>
#include <gongvariant.h>
#include "testcsv.h"

using namespace gong;

TestCSV::TestCSV()

{
}


TestCSV::~TestCSV()
{
}

int TestCSV::run()
{
	Xtring content = "\"NOMBRE\",\"LANGUAGE\"";
	XtringList tokens;
//	CsvUtils::tokenize( tokens, content, "\"", "," );
//	_GONG_DEBUG_ASSERT( tokens[0] == "NOMBRE" );
//	_GONG_DEBUG_ASSERT( tokens[1] == "LANGUAGE" );
//	tokens.clear();
	content = "\"Santi \"\"Santilín\"\"\",\"c++\"";
	CsvUtils::tokenize( tokens, content, '\"', ',' );
	_GONG_DEBUG_ASSERT( tokens[0] == "Santi \"Santilín\"" );
	_GONG_DEBUG_ASSERT( tokens[1] == "c++" );

	return 1;
}


