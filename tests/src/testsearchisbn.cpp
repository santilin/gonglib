#include <gongdebug.h>
#include <gongmoney.h>
#include <gongvariant.h>
#include "../../gong-editorial/editorialisbnsearch.h"
#include "testsearchisbn.h"


#include "../../gong-editorial/editorialisbnsearch.cpp"

using namespace gong;

TestSearchISBN::TestSearchISBN()

{
}


TestSearchISBN::~TestSearchISBN()
{
}

int TestSearchISBN::run()
{
	_GONG_DEBUG_TRACE(0);
	editorial::ISBNInfo isbninfo;
	Xtring ret = ISBNSearch("mcu.es", "8426428320", isbninfo);
	_GONG_DEBUG_PRINT(0, isbninfo.isbn10 ); 

	return 1;
}


