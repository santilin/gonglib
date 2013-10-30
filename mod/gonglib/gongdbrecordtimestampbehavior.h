#ifndef _GONG_DBRECORD_TIMESTAMP_BEHAVIOR_H
#define _GONG_DBRECORD_TIMESTAMP_BEHAVIOR_H

/** @file gongdbrecortimestampbehavior.h A behavior that adds timestamp fields to records
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongxtring.h"
#include "gongdbrecordbehavior.h"

namespace gong {

class dbRecordTimestampBehavior: public dbRecordBehavior
{
public:
    dbRecordTimestampBehavior( const Xtring &fldname_created = "REC_CREATED",
                               const Xtring &fldname_updated = "REC_UPDATED" )
        : dbRecordBehavior(), mFldCreated( fldname_created ), mFldUpdated( fldname_updated )
    {}

    virtual bool addTo( dbTableDefinition *tbldef );
    virtual bool save(const dbRecord *old_record,
                      dbRecord *therecord, bool isbefore, bool &result);

private:
    Xtring mFldCreated, mFldUpdated;
};

} // namespace gong

#endif // _GONG_DBRECORD_TIMESTAMP_BEHAVIOR_H
