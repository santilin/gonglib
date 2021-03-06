#include <gongregconfig.h>
#include "testcommon.h"
#include "testregconfig.h"

using namespace gong;

TestRegConfig::TestRegConfig()
{
}


TestRegConfig::~TestRegConfig()
{
}


void TestRegConfig::run()
{
	_GONG_DEBUG_TRACE(0);
	testNumberMasks();
	std::cout << "TestRegConfig finished" << std::endl;
}

void TestRegConfig::testNumberMasks()
{
	_GONG_DEBUG_TRACE(1);
	RegConfig rc;
	rc.setLocale("es_ES.utf8");
	_GONG_DEBUG_PRINT( 0, rc.getCurrencyMask() );
	_GONG_DEBUG_PRINT( 0, rc.getNumberMask() );
}



