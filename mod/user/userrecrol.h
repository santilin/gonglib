/*<<<<<ROL_PREAMBLE*/
#ifndef _USER_USERRECROL_H
#define _USER_USERRECROL_H
/*>>>>>ROL_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file userrecrol.h Registro de roles del módulo user
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
#include <dbappdbapplication.h>

/*<<<<<ROL_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>ROL_INCLUDES*/

namespace gong {
namespace user {


/*<<<<<ROL_CONSTRUCTOR*/
class RecRol: public dbRecord
{
public:
	RecRol(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("ROL"), recid, user)
/*>>>>>ROL_CONSTRUCTOR*/
	{};
}; // end class

/*<<<<<ROL_POSTAMBLE*/
} // namespace user
} // namespace gong
#endif // USER_USERRECROL_H
/*>>>>>ROL_POSTAMBLE*/

