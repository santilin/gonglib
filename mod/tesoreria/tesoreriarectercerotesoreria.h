/*<<<<<TERCEROTESORERIA_PREAMBLE*/
#ifndef _TESORERIA_TESORERIARECTERCEROTESORERIA_H
#define _TESORERIA_TESORERIARECTERCEROTESORERIA_H
/*>>>>>TERCEROTESORERIA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file tesoreriarectercerotesoreria.h Registro de
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*//*<<<<<TERCEROTESORERIA_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>TERCEROTESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {


/*<<<<<TERCEROTESORERIA_CONSTRUCTOR*/
class RecTerceroTesoreria: public dbRecord
{
public:
	RecTerceroTesoreria(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("TERCEROTESORERIA"), recid, user)
/*>>>>>TERCEROTESORERIA_CONSTRUCTOR*/
	{};
}; // end class

/*<<<<<TERCEROTESORERIA_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIARECTERCEROTESORERIA_H
/*>>>>>TERCEROTESORERIA_POSTAMBLE*/

