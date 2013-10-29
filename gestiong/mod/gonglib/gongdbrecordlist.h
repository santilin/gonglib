#ifndef _GONG_DBRECORDLIST_H
#define _GONG_DBRECORDLIST_H

/** @file gongdbrecordlist.h A list of dbRecords
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <vector>
#include "gongdbrecord.h"

namespace gong
{

class dbRecordList: public std::vector<dbRecord *>
{
public:
    dbRecordList() : std::vector<dbRecord *>() {}
    ~dbRecordList() {
        clear();
    }
    void clear();
    dbRecord *getRecord(unsigned int i) const;
    Variant getValue(unsigned int i, const Xtring &fldname) const
    {
        return operator[](i)->getValue(fldname);
    }
    bool setValue(unsigned int i, const Xtring &fldname, const Variant &val)
    {
        return operator[](i)->setValue(fldname, val);
    }
    dbRecord *addRecord(const dbRecord *rec, int pos=-1);
    void removeRecord(unsigned int index)
    {
        erase( begin() + index );
    }
//     int getFieldPos(const Xtring &fldname) const
// 		{ return operator[](0)->getFieldPos(fldname); }
#ifdef _GONG_DEBUG
    Xtring toString(int format = TOSTRING_DEBUG) const;
#endif
};

}; // namespace gong

#endif // _GONG_DBRECORDLIST_H
