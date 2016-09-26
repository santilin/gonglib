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

#ifdef HAVE_BOOST_REGEX

#include <boost/regex.hpp>

namespace gong {

typedef boost::csub_match RegExpMatch;
typedef boost::cmatch RegExpMatchList;
typedef boost::sregex_iterator RegExpIterator;

class RegExp: public boost::regex
{
public:
    RegExp( const std::string &pattern) : boost::regex(pattern) {}
    /** Busca una subcadena */
    bool search( const char *subject, RegExpMatchList &matches ) const;
    bool searchAll( const std::string &subject, RegExpMatchList &matches ) const;
	/** Determina si la cadena completa concuerda */
    bool match( const char *subject, RegExpMatchList &matches ) const;
	std::string replaceAll( const std::string &subject, const std::string &fmt) const;
	std::string replaceFirst( const std::string &subject, const std::string &fmt) const;
};

} // namespace gong


#elif defined( HAVE_POCOLIB )

#include <Poco/RegularExpression.h>

namespace gong {

typedef Poco::RegularExpression RegExp;
typedef Poco::RegularExpression::Match RegExpMatch;
typedef Poco::RegularExpression::MatchVec RegExpMatchList;

}; // namespace gong


#else

#include "gongxtring.h"
typedef struct {
    int offset;
    int length;
} RegExpMatch;
typedef std::vector<RegExpMatch> RegExpMatchList;

class RegExp {
public:
    RegExp() {};
};

#endif

#endif
