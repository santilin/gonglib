#ifndef _GONG_REGEXP_H
#define _GONG_REGEXP_H

/** @file gongregexp.h Regular expressions (\todo)
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#ifdef HAVE_POCOLIB
#include <Poco/RegularExpression.h>

namespace gong {	

typedef Poco::RegularExpression RegExp;
typedef Poco::RegularExpression::Match RegExpMatch;
}; // namespace gong


#else

#include "gongxtring.h"
typedef struct { int offset; int length; } RegExpMatch;
class RegExp {
public:
	RegExp() {};
};

#endif

#endif
