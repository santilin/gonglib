#ifndef _GONG_DBRESULTSETDATAMODEL_H
#define _GONG_DBRESULTSETDATAMODEL_H

/** @file gongdbrecordlistdatamodel.h A data model between resultsets and GUI table widgets
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongdbresultset.h"
#include "gongdbrecorddatamodel.h"

namespace gong
{

class dbResultSetDataModel: public dbRecordDataModel
{
public:
    dbResultSetDataModel( dbResultSet *res, dbRecord *record );
    virtual ~dbResultSetDataModel();

    virtual Variant getValue( unsigned int row, unsigned int col ) const;
    virtual bool setValue( unsigned int row, unsigned int col, const Variant &value );
    virtual unsigned int getRowCount() const {
        return pResultSet->getRowCount();
    }
    virtual bool setViewIndex( int nview );
    const dbResultSet *getResultSet() const {
        return pResultSet;
    }
    void getRecord( Xtring arg1 );

private:
    dbResultSet *pResultSet;
    dbRecord *pRecord;
    dbViewDefinition *pViewDefinition;
};

}; // namespace gong

#endif // _GONG_DBRESULTSETDATAMODEL_H

