/*<<<<<CUENTATESORERIA_PREAMBLE*/
#ifndef _TESORERIA_TESORERIARECCUENTATESORERIA_H
#define _TESORERIA_TESORERIARECCUENTATESORERIA_H
/*>>>>>CUENTATESORERIA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file tesoreriareccuentatesoreria.h Registro de cuentas de tesorería
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

/*<<<<<CUENTATESORERIA_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>CUENTATESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {

/*<<<<<CUENTATESORERIA_CONSTRUCTOR*/
class RecCuentaTesoreria: public dbRecord
{
public:
	RecCuentaTesoreria(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("CUENTATESORERIA"), recid, user)
/*>>>>>CUENTATESORERIA_CONSTRUCTOR*/
	{};

/*<<<<<CUENTATESORERIA_MEMBERS*/
	void init();
/*>>>>>CUENTATESORERIA_MEMBERS*/

}; // class

/*<<<<<CUENTATESORERIA_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIARECCUENTATESORERIA_H
/*>>>>>CUENTATESORERIA_POSTAMBLE*/
