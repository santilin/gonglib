#ifndef CAPELCAPELCSSMODULE_H_
#define CAPELCAPELCSSMODULE_H_

#include "capelmodule.h"

namespace capel {


class CssModule: public CapelModule
{
public:
	CssModule( const Xtring &filename,
	                const Xtring &begin_keyword = "<<<<<", const Xtring &end_keyword = ">>>>>",
	                const Xtring &begin_comment = "/*", const Xtring &end_comment = "*/" )
		: CapelModule(filename, begin_keyword, end_keyword, begin_comment, end_comment)
		{ 
			mParameterDelimiterBegin = "¿";
			mParameterDelimiterEnd = "?";
		}
};

}

#endif

