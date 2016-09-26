#include <gongdebug.h>
#include <gongregexp.h>
#include "httpserverbladeinterpreter.h"


namespace gong {
namespace httpserver {

const char *replacements[][2] = {
	{ "{{\\s*asset\\(['\"](/*)(.*)['\"]\\);*\\s*}}", "$BASEURL/\\2" }
};
	

Xtring BladeInterpreter::interpret ( const Xtring& source, const std::map<Xtring, Xtring> &variables )
{
	Xtring ret = source;
	uint i = 0;
	try {
		for( i=0; i<sizeof(replacements)/sizeof(replacements[0]); ++i) {
			RegExp repl_reg_exp = RegExp(replacements[i][0]);
			ret = repl_reg_exp.replaceAll( ret, replacements[i][1] );
		}
	} catch( std::exception &e ) {
		_GONG_DEBUG_PRINT(0, Xtring(e.what()) + " reemplazando la exp. reg ");
	}
 	for( auto &v : variables ) {
		ret = ret.replace("$" + v.first, v.second );
 	}
    return ret;
}


} // namespace httpserver
} // namespace gong
