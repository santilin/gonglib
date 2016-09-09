/*<<<<<USUARIA_PREAMBLE*/
#ifndef _USER_USERRECUSUARIA_H
#define _USER_USERRECUSUARIA_H
/*>>>>>USUARIA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file userrecusuaria.h Registro de
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

/*<<<<<USUARIA_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>USUARIA_INCLUDES*/

namespace gong {
namespace user {


/*<<<<<USUARIA_CONSTRUCTOR*/
class RecUsuaria: public dbRecord
{
public:
	RecUsuaria(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("USUARIA"), recid, user)
/*>>>>>USUARIA_CONSTRUCTOR*/
	{};
}; // end class

/*<<<<<USUARIA_POSTAMBLE*/
} // namespace user
} // namespace gong
#endif // USER_USERRECUSUARIA_H
/*>>>>>USUARIA_POSTAMBLE*/

