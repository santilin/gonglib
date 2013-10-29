#ifndef _GONG_DBRECORDLISTDATAMODEL_H
#define _GONG_DBRECORDLISTDATAMODEL_H

/** @file gongdbrecordlistdatamodel.h A data model between record lists and GUI table widgets
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongdbrecordlist.h"
#include "gongdbrecorddatamodel.h"

namespace gong {

class dbRecordListDataModel: public dbRecordDataModel
{
public:
    dbRecordListDataModel(dbRecordList *reclist, dbRecord *record,
                          const dbViewDefinitionsList &vlist, const Xtring &filter = Xtring());
    virtual ~dbRecordListDataModel();

    virtual Variant getValue(unsigned int row, unsigned int col) const;
    virtual bool setValue(unsigned int row, unsigned int col, const Variant &value);
    virtual unsigned int getRowCount() const;
    virtual bool setViewIndex(int nview);
    const dbRecordList *getRecordList() const {
        return pRecordList;
    }

private:
    dbRecordList *pRecordList;
    dbRecord *pRecord;
};

}; // namespace gong

#endif // GONGRECORDLISTDATAMODEL

