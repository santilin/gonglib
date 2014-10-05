/*<<<<<TIPOAPUNTETESORERIA_PREAMBLE*/
#ifndef _TESORERIA_TESORERIARECTIPOAPUNTETESORERIA_H
#define _TESORERIA_TESORERIARECTIPOAPUNTETESORERIA_H
/*>>>>>TIPOAPUNTETESORERIA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file tesoreriarectipoapuntetesoreria.h Registro de
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

/*<<<<<TIPOAPUNTETESORERIA_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>TIPOAPUNTETESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {


/*<<<<<TIPOAPUNTETESORERIA_CONSTRUCTOR*/
class RecTipoApunteTesoreria: public dbRecord
{
public:
	RecTipoApunteTesoreria(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("TIPOAPUNTETESORERIA"), recid, user)
/*>>>>>TIPOAPUNTETESORERIA_CONSTRUCTOR*/
    {};
}; // end class

/*<<<<<TIPOAPUNTETESORERIA_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIARECTIPOAPUNTETESORERIA_H
/*>>>>>TIPOAPUNTETESORERIA_POSTAMBLE*/

