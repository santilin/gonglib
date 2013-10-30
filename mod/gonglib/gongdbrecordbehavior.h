#ifndef _GONG_DBRECORDBEHAVIOR_H
#define _GONG_DBRECORDBEHAVIOR_H

/** @file gongdbrecordbehavior.h A behavior to add to records
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gonglist.h>

#ifndef BEHAVIOR_PRE
#define BEHAVIOR_PRE true
#define BEHAVIOR_POST false
#endif

namespace gong {

class dbRecord;
class dbTableDefinition;

class dbRecordBehavior
{
public:
    dbRecordBehavior() {}

    virtual bool addTo( dbTableDefinition *tbldef ) {
        return true;
    }
    virtual void clear(dbRecord *therecord, bool setcustomvalues ) {}
    virtual bool read(dbRecord *therecord, bool is_pre, const Xtring &sql) {
        return true;
    }
    virtual bool remove(dbRecord *therecord, bool is_pre ) {
        return true;
    }
    virtual bool save(dbRecord *therecord, bool is_pre, bool &result) {
        result = true;
        return true;
    }
    bool beforeSave(dbRecord *therecord, bool &result) {
        return save(therecord, BEHAVIOR_PRE, result );
    }
    bool afterSave(dbRecord *therecord, bool &result) {
        return save(therecord, BEHAVIOR_POST, result );
    }

private:
    dbTableDefinition *pTableDef;
};

typedef List<dbRecordBehavior *> dbRecordBehaviorsList;

} // namespace gong

#endif // DBRECORDBEHAVIOR_H
