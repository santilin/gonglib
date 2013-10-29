#include <gongdebug.h>
#include <gongmoney.h>
#include <gongvariant.h>
#include <gonghttpclient.h>
#include "testhttpclient.h"


using namespace gong;

TestHTTPClient::TestHTTPClient()

{
}


TestHTTPClient::~TestHTTPClient()
{
}

int TestHTTPClient::run()
{
	_GONG_DEBUG_TRACE(0);
	XtringList response;
	char *content = 0;
	int res = gong::read_http_file( response, &content, "localhost",
									"/webs/lospajaros.org/web/register_user.php",
									Xtring::null, "POST", "" );
	_GONG_DEBUG_PRINT(0, res );
	_GONG_DEBUG_PRINT(0, response.join("\n") );
	_GONG_DEBUG_PRINT(0, content );
	if( content )
		delete content;
	return 1;
}


