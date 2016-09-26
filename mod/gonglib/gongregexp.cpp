#include "gongregexp.h"

namespace gong {

	
#ifdef HAVE_BOOST_REGEX
	
bool RegExp::search( const char *subject, RegExpMatchList &matches ) const
{
    return regex_search( subject, matches, *this );
}

bool RegExp::match( const char *subject, RegExpMatchList &matches ) const
{
    return regex_match( subject, matches, *this );
}

/*
 * Ejemplos:
 * std::string subject = "Stack\nover\rflowâ€";
 * boost::regex expr("(\\n)|(\\r)|([^\x20-\x7E])");
 * std::string fmt("(?1nl)(?2nl)"); // Omitted the (?3) as a no-op
 */
std::string RegExp::replaceAll( const std::string &subject, const std::string &fmt) const
{
	return boost::regex_replace(subject, *this, fmt, boost::match_default | boost::format_all);
}

#elif defined( HAVE_POCOLIB )

#else
// @todo
RegExp::RegExp( /*const std::string &pattern*/ )
{
	throw new std::runtime_error("No hay soporte para expresiones regulares en gestiong. Usa boost::regexp o las pocolib");
}

#endif

	
}
