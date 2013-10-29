#ifndef _GONG_DBERROR_H
#define _GONG_DBERROR_H

/** @file gongdberror.h dbConnection related exceptions
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <stdexcept>
#include "gongxtring.h"

namespace gong {

class dbError: public std::runtime_error
{
public:
    dbError(const Xtring &message, int number = 0, const Xtring &query = Xtring::null)
        : std::runtime_error(message), mNumber(number), mQuery(query)
    {}
    ~dbError() throw() {};
    int getNumber() const {
        return mNumber;
    }
    const Xtring &getQuery() const {
        return mQuery;
    }
    Xtring getWholeError() const {
        return Xtring(what()) + "\n" + mQuery;
    }
private:
    int mNumber;
    Xtring mQuery;
};

}; // namespace gong

#endif
