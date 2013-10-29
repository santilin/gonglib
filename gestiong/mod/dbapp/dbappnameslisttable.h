#ifndef _DBAPP_NAMESLISTTABLE_H_
#define _DBAPP_NAMESLISTTABLE_H_

/** @file dbappnameslisttable.h Field and table definition for nameslist fields
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongdbmastertable.h>
#include <gongdbfieldlistofvalues.h>
#include <gongdbrecord.h>

namespace gong {

class NamesListTable: public dbMasterTable
{
public:
    NamesListTable(dbDefinition &db, const Xtring &name);
};


class FldNamesListTable: public dbFieldListOfValues<int>
{
public:
    FldNamesListTable( const Xtring &tablename, const Xtring& fldname,
                       dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                       const Xtring &defaultvalue = Xtring::null );
    virtual bool isValid( dbRecord *r, dbFieldValue *value,
                          ValidResult::Context context, ValidResult *integres) const; // from dbFieldDefinition
    void fill( dbConnection &conn );
    void fill( XtringList &mCaptions, List<int> &mValues );
    int findCode( const Xtring &name ) const;
protected:
    XtringList mCaptions;
    List<int> mValues;
};

class RecNamesListTable: public dbRecord
{
public:
    RecNamesListTable(const Xtring &tablename, dbConnection *conn, dbRecordID recid=0, dbUser *user=0);
    virtual bool save(bool saverelated); // from dbRecord
    virtual bool remove(); // from dbRecord
    int findCode( const Xtring &name ) const;
private:
    void updateDatabaseFields();
};

} // namespace gong

#endif // _DBAPP_NAMESLISTTABLE_H_
