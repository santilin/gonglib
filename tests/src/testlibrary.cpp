#include <gonglibrary.h>

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestLibrary )
using namespace gong;

BOOST_AUTO_TEST_CASE( TestConstructorSinParametros )
{
	__gong_debug::_gong_debug_level = -1;
// 	BOOST_CHECK_THROW( GongLibrary l("", "", ""), std::runtime_error);
}

BOOST_AUTO_TEST_CASE( TestConstructorConParametros )
{
	GongLibrary l("/usr/share/", "gestiong", "");
	BOOST_CHECK_EQUAL( l.getGonglibDataDir(), "/usr/share/gonglib/" );
	BOOST_CHECK_EQUAL( l.getGlobalDataDir(), "/usr/share/gestiong/" );
	BOOST_CHECK_EQUAL( l.getLocalDataDir(), "/home/santilin/.gonglib/gestiong/" );
}


BOOST_AUTO_TEST_SUITE_END()
