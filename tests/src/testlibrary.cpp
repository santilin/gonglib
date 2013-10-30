//
// C++ Implementation: testlibrary
//
// Description:
//
//
// Author: Santiago Capel <elcansino@arremolina.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <gonglibrary.h>
#include <gongdebug.h>
#include "testlibrary.h"

using namespace gong;

TestLibrary::TestLibrary()
{
}


TestLibrary::~TestLibrary()
{
}

void TestLibrary::run()
{
	GongLibrary l("", "", "");
//	_GONG_DEBUG_ASSERT_EQUALS( l.getHostIP("localhost"), "127.0.1.1" );
//	_GONG_DEBUG_ASSERT_EQUALS( l.getInterfaceIP("eth0"), "84.121.119.151" );
	_GONG_DEBUG_PRINT(0, "TestLibrary finished" );
}
