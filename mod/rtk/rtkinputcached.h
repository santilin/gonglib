#ifndef RTKINPUTCACHED_H
#define RTKINPUTCACHED_H

/*<<<<<COPYLEFT*/
/** @file rtkinputcached.h Cached input object
 * RTK The report toolkit. Copyright (C) 2003-2013 Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

#include <db_185.h>
#include "rtkinput.h"


namespace gong {}
namespace RTK = gong;

namespace gong {

class InputCached: public Input
{
public:
    InputCached(class Report &r, const char *basename = 0, const char *name = 0);
    InputCached(class Report &r, class OutputCached *outputcached, const char *name = 0);
    ~InputCached();

    virtual bool init();
    virtual bool next();
    virtual Variant getValue(uint sourcepos, Variant::Type type) const;
    const void *key() const {
        return mKey.data;
    }
    const void *data() const {
        return mData.data;
    };

private:
    char *pBaseName;
    DB *pDB;
    DBT mKey, mData;
    bool mDeleteDB;
};

} // namespace

#endif
