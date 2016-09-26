#include <gongdate.h>

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestDate )
using namespace gong;

BOOST_AUTO_TEST_CASE( TestConstructor )
{
    Date dateTest;
	BOOST_TEST( dateTest.isNull() == true );
}

BOOST_AUTO_TEST_SUITE_END()


