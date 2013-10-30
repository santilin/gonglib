/*<<<<<STRINGAUTOINC_PREAMBLE*/
#ifndef ___DBAPPFLDSTRINGAUTOINC_H
#define ___DBAPPFLDSTRINGAUTOINC_H
/*>>>>>STRINGAUTOINC_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file dbappfldstringautoinc.h dbFieldDefinition para strings con autoincremento
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

/*<<<<<DBFIELD_STRINGAUTOINC_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_STRINGAUTOINC_INCLUDES*/

namespace gong {

class FldStringAutoInc: public dbFieldDefinition
{
public:
    FldStringAutoInc(dbConnection *conn, const Xtring &tablename, const Xtring &name,
                     int width, dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                     const Xtring &defaultvalue = Xtring::null)
        : dbFieldDefinition(tablename, name, SQLSTRING, width, 0, flags, defaultvalue),
          pConn( conn )
    {}

    virtual Variant customDefaultValue() const; // from dbFieldDefinition
private:
    dbConnection *pConn;
}; // class

/*<<<<<STRINGAUTOINC_POSTAMBLE*/
} // namespace gong
#endif // __DBAPPFLDSTRINGAUTOINC_H
/*>>>>>STRINGAUTOINC_POSTAMBLE*/



