#include <gongregexp.h>
#include "httpserverbladeinterpreter.h"


namespace gong
{
namespace httpserver
{

const char *replacements[][2] = {
	{ "\\sasset\\(['\"](.*)['\"]\\)\\s", "$BASEURL/1" }
};
	

Xtring BladeInterpreter::interpret ( const Xtring& source, const Dictionary<Xtring> &variables )
{
	Xtring ret = source;
	uint i = 0;
	try {
		for( i=0; i<sizeof(replacements); ++i) {
			RegExp repl_reg_exp = RegExp(replacements[i][0]);
			ret = repl_reg_exp.replaceAll( ret, replacements[i][1] );
		}
	} catch( std::exception &e ) {
		_GONG_DEBUG_PRINT(0, Xtring(e.what()) + " reemplazando la exp. reg ");
	}
// 	for( auto v : variables ) {
// _GONG_DEBUG_TRACE(0);
// 		ret = ret.replace("$" + v.first, v.second );
// 	}
    return ret;
}


} // namespace httpserver
} // namespace gong
