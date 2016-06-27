#include <gongsettings.h>
#include "testcommon.h"
#include "testsettings.h"

using namespace gong;

TestSettings::TestSettings()
{
}


TestSettings::~TestSettings()
{
}


void TestSettings::run()
{
	testStringOfSettings();
	std::cout << "TestSettings finished" << std::endl;
}

void TestSettings::testStringOfSettings()
{
	static const char *global_settings =
		"intvalue = 1;\n"
		"doublevalue = 1.3;\n"
		"stringvalue = \"a \\\nstring\";\n"
		"datevalue = 2004-10-12;\n"
		"timevalue = 12:49;\n";

	static const char *local_settings =
		"intvalue = 3;\n"
		"stringvalue = \"another \\\nstring\";\n"
		"datevalue = 2004-10-12;\n"
		"timevalue = 10:34;\n";


	Settings s("global_settings");
	_GONG_DEBUG_ASSERT( s.readString( global_settings ) );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "intvalue" ).toString(), "1" );
	_GONG_DEBUG_ASSERT( s.getValue( "intvalue" ).toInt() == 1 );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "doublevalue" ).toString(), "1.3" );
	_GONG_DEBUG_ASSERT( s.getValue( "doublevalue" ).toDouble() == 1.3 );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "stringvalue" ).toString(), "a string" );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "datevalue" ).toString(), "2004-10-12" );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "datevalue" ).toDate().toString(), Date(2004,10,12).toString() );
	_GONG_DEBUG_ASSERT( s.getValue( "datevalue" ).toDate() == Date(2004,10,12) );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "timevalue" ).toString(), "12:49" );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "timevalue" ).toTime().toString(), Time(12,49).toString() );
	_GONG_DEBUG_ASSERT( s.getValue( "timevalue" ).toTime() == Time(12,49) );

	_GONG_DEBUG_ASSERT( s.readString( local_settings ) );
// 	_GONG_DEBUG_PRINT( 0, s.getValue( "intvalue" ).toString() );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "intvalue" ).toString(), "3" );
	_GONG_DEBUG_ASSERT( s.getValue( "intvalue" ).toInt() == 3 );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "doublevalue" ).toString(), "1.3" );
	_GONG_DEBUG_ASSERT( s.getValue( "doublevalue" ).toDouble() == 1.3 );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "stringvalue" ).toString(), "another string" );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "datevalue" ).toString(), "2004-10-12" );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "datevalue" ).toDate().toString(), Date(2004,10,12).toString() );
	_GONG_DEBUG_ASSERT( s.getValue( "datevalue" ).toDate() == Date(2004,10,12) );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "timevalue" ).toString(), "10:34" );
	_GONG_DEBUG_ASSERT_EQUALS( s.getValue( "timevalue" ).toTime().toString(), Time(10,34).toString() );
	_GONG_DEBUG_ASSERT( s.getValue( "timevalue" ).toTime() == Time(10,34) );


}



