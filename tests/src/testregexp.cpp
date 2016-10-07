#include <gongregexp.h>
#include <iostream>

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestRegExp )
using namespace gong;

BOOST_AUTO_TEST_CASE( Basic )
{
	RegExpMatchList matches;
	RegExp re("[0-9]");
	BOOST_CHECK( re.search("2", matches) );
	BOOST_CHECK( matches.size() == 1 );
	BOOST_CHECK( matches[0] == "2" );
	BOOST_CHECK( !re.search("a", matches) );

	RegExp re2("\\s*[0-9]");
	BOOST_CHECK( re2.search(" 2", matches) );
	BOOST_CHECK( matches.size() == 1 );
	BOOST_CHECK( matches[0] == " 2" );
	BOOST_CHECK( re2.search("    2", matches) );
	BOOST_CHECK( re2.search("     \t 2", matches) );
	BOOST_CHECK( !re2.search("a", matches) );

	{
		RegExp re("\\s*([0-9])");
		BOOST_CHECK( re.search(" 2", matches) );
		BOOST_CHECK( matches.size() == 2 );
		BOOST_CHECK( matches[0] == " 2" );
		BOOST_CHECK( matches[1] == "2" );
		BOOST_CHECK( re.search("    2", matches) );
		BOOST_CHECK( re.search("     \t 2", matches) );
		BOOST_CHECK( !re.search("a", matches) );
	}
	{
		RegExp re("\\s*asset");
		BOOST_CHECK( re.search(" asset", matches) );
		BOOST_CHECK( re.search("    asset", matches) );
		BOOST_CHECK( re.search("     \t asset", matches) );
		BOOST_CHECK( re.search("     \t gasset", matches) );
		BOOST_CHECK( !re.search("a", matches) );
	}
	{
		RegExp re("^asset$");
		BOOST_CHECK( re.search("asset", matches) );
		BOOST_CHECK( !re.search("    asset", matches) );
		BOOST_CHECK( !re.search("gasset", matches) );
		BOOST_CHECK( !re.search("a", matches) );
	}

	{
		RegExp re("\\s*asset\\(");
		BOOST_CHECK( re.search("asset(", matches) );
		BOOST_CHECK( re.search(" asset(", matches) );
		BOOST_CHECK( re.search("df  dfdfasset(", matches) );
		BOOST_CHECK( !re.search("    aset", matches) );
	}
	
	{
		RegExp re("\\s*asset\\(");
		BOOST_CHECK( re.search("asset(", matches) );
		BOOST_CHECK( matches.size() == 1 );
		BOOST_CHECK( matches[0] == "asset(" );
		BOOST_CHECK( re.search(" asset(", matches) );
		BOOST_CHECK( re.search("df  dfdfasset(", matches) );
		BOOST_CHECK( !re.search("    aset", matches) );
	}

	{
		RegExpMatchList matches;
		RegExp repl_reg_exp("\\s*asset\\(['\"]([a-z]*)['\"]\\)\\s*");
		BOOST_CHECK( repl_reg_exp.search("____   asset('hola') ", matches) );
		BOOST_CHECK( matches.size() == 2 );
		BOOST_CHECK( matches.str(0) == "   asset('hola') " );
		BOOST_CHECK( matches.str(1) == "hola" );
	}
	
	
}

BOOST_AUTO_TEST_SUITE_END()