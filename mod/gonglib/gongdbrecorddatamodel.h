#ifndef _GONG_DBRECORDDATAMODEL_H
#define _GONG_DBRECORDDATAMODEL_H

/** @file gongdbrecorddatamodel.h A data model between records and GUI table widgets
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongtabledatamodel.h"
#include "gongdbconnection.h"
#include "gongdbrecord.h"

namespace gong
{

class dbRecordDataModel: public TableDataModel
{
public:
    dbRecordDataModel( dbRecord *record, const dbViewDefinitionDict &vlist,
                       const Xtring &filter = Xtring() );
    virtual ~dbRecordDataModel() {
        if ( pResultSet ) delete pResultSet;
    }
    virtual Variant getValue( unsigned int row, unsigned int col ) const;
    virtual bool setValue( unsigned int /*row*/, unsigned int /*col*/, const Variant &/*value*/ ) {
        return false;
    }
    virtual bool setViewIndex( int nview ); // reimpl
    virtual unsigned int getRowCount() const;
    dbRecord *getRecord() const {
        return pRecord;
    }

private:
    dbRecord *pRecord;
    dbResultSet *pResultSet;
};

}; // namespace gong

#endif // _GONG_DBRECORDDATAMODEL_H
