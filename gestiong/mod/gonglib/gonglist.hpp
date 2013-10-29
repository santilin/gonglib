/** @file gonglist.hpp Templatized list of values
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongxtring.h>
#include <gonglist.h>
#include <gongdbconnection.h>

/**
       @class List
       @brief Templatized list of values

	For alternatives to template instantiation, see:

	http://www.cs.cmu.edu/cgi-bin/info2www?(gcc.info)Template%20Instantiation
*/

namespace gong {

template<>
std::string List<long>::join(const std::string &sep) const
{
    std::string result;
    for( const_iterator it = begin(); it != end(); ++it ) {
        if( result.size() )
            result += sep;
        result += Xtring::number(*it);
    }
    return result;
}

template<>
std::string List<int>::join(const std::string &sep) const
{
    std::string result;
    for( const_iterator it = begin(); it != end(); ++it ) {
        if( result.size() )
            result += sep;
        result += Xtring::number(*it);
    }
    return result;
}

template<>
std::string List<dbRecordID>::join(const std::string &sep) const
{
    std::string result;
    for( const_iterator it = begin(); it != end(); ++it ) {
        if( result.size() )
            result += sep;
        result += Xtring::number(*it);
    }
    return result;
}
}

