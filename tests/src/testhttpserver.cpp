#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include <httpserverserver.h>
#include <httpserverbladeinterpreter.h>

BOOST_AUTO_TEST_SUITE( TestHttpServer )
using namespace gong;
using namespace gong::httpserver;

BOOST_AUTO_TEST_CASE( TestBladeInterpreter )
{
	std::map<Xtring, Xtring> variables;
	variables["BASEURL"]="http://www.gestiong.org";
	Xtring source;
	source = "<HTML><BODY></BODY></HTML>";
	Xtring not_changed( BladeInterpreter::interpret(source, variables) );
	BOOST_CHECK( not_changed == source );
	source = "<HTML><BODY><A href='$BASEURL'></BODY></HTML>";
	Xtring changed( BladeInterpreter::interpret(source, variables) );
	BOOST_CHECK( changed  == "<HTML><BODY><A href='http://www.gestiong.org'></BODY></HTML>" );
	source = "<HTML><BODY><A href='{{asset('/index.html')}}'></BODY></HTML>";
	Xtring changed_asset( BladeInterpreter::interpret(source, variables) );
	BOOST_CHECK( changed_asset  == "<HTML><BODY><A href='http://www.gestiong.org/index.html'></BODY></HTML>" );
	source = "<HTML><BODY><A href='{{asset('index.html')}}'></BODY></HTML>";
	Xtring changed_asset_no_slash( BladeInterpreter::interpret(source, variables) );
	BOOST_CHECK( changed_asset_no_slash == "<HTML><BODY><A href='http://www.gestiong.org/index.html'></BODY></HTML>" );
}

BOOST_AUTO_TEST_SUITE_END()
