/*<<<<<USER_PREAMBLE*/
#ifndef _USER_USERRECUSER_H
#define _USER_USERRECUSER_H
/*>>>>>USER_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file userrecuser.h Registro de usuarias
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

/*<<<<<USER_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>USER_INCLUDES*/

namespace gong {
namespace user {


/*<<<<<USER_CONSTRUCTOR*/
class RecUser: public dbRecord
{
public:
	RecUser(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("USER"), recid, user)
/*>>>>>USER_CONSTRUCTOR*/
	{};
}; // end class

/*<<<<<USER_POSTAMBLE*/
} // namespace user
} // namespace gong
#endif // USER_USERRECUSER_H
/*>>>>>USER_POSTAMBLE*/

