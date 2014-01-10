#ifndef RTKOUTPUTCACHED_H
#define RTKOUTPUTCACHED_H

/*<<<<<COPYLEFT*/
/** @file rtkoutputcached.h Cached output object
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

#ifdef HAVE_BDB
#include <db_185.h>
#else
typedef int DB;
typedef struct { void *data; } DBT;
#endif

#include "rtkinput.h"
#include "rtkoutput.h"

namespace gong {

class OutputCached: public Output
{
public:
    enum CacheType { CacheFile, CacheMemory };
    OutputCached(class Report &r, const Input *pInput, CacheType cachetype,
                 const char *basename = 0);
    ~OutputCached();

    virtual int startReport();
    virtual int endReport();
    virtual Measure startPage() {
        return 0.0;
    }
    virtual Measure endPage() {
        return 0.0;
    }
    virtual Measure startSection(const Section &) {
        return 0.0;
    }
    virtual Measure endSection(const Section &section);
    virtual Measure printObject(const Object &) {
        return 0.0;
    }
    int writeRecord(void *keyptr, int keylen, void *recptr, int reclen);
    DB *getDB() const {
        return pDB;
    }

private:
    const Input *pInput;
    CacheType mCacheType;
    const char *pBaseName;
    DB *pDB;
};

} // namespace

#endif
