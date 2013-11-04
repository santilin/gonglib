/*<<<<<METADBDATA_PREAMBLE*/
#ifndef ___DBAPPRECMETADBDATA_H
#define ___DBAPPRECMETADBDATA_H
/*>>>>>METADBDATA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file dbapprecmetadbdata.h Record of meta data for the database
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

/*<<<<<METADBDATA_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>METADBDATA_INCLUDES*/

namespace gong {

/*<<<<<METADBDATA_CONSTRUCTOR*/
class RecMetaDBData: public dbRecord
{
public:
	RecMetaDBData(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("METADBDATA"), recid, user)
/*>>>>>METADBDATA_CONSTRUCTOR*/
    {};
    bool lock(const Xtring &user, const Xtring &password, const Xtring &desc, Date date);
    bool unlock(const Xtring &password);
}; // end class

/*<<<<<METADBDATA_POSTAMBLE*/
} // namespace gong
#endif // __DBAPPRECMETADBDATA_H
/*>>>>>METADBDATA_POSTAMBLE*/

