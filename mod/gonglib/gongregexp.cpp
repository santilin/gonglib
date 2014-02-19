#include "gongregexp.h"

namespace gong {

bool RegExp::search( std::string subject, RegExpMatchList &matches )
{
	return regex_search( subject, matches, *this );
}	

bool RegExp::match( std::string subject, RegExpMatchList &matches )
{
	return regex_match( subject, matches, *this );
}
	
}
