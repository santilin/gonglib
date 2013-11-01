/*<<<<<COPYLEFT*/
/** @file gastosrecgasto.h Registro de gastos e ingresos
 * The GestiONG project. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<GASTO_PREAMBLE*/
#ifndef _GASTOS_GASTOSRECGASTO_H
#define _GASTOS_GASTOSRECGASTO_H
/*>>>>>GASTO_PREAMBLE*/

/*<<<<<GASTO_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>GASTO_INCLUDES*/

namespace gong {
namespace gastos {

/*<<<<<GASTO_CONSTRUCTOR*/
class RecGasto: public dbRecord
{
public:
	RecGasto(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("GASTO"), recid, user)
/*>>>>>GASTO_CONSTRUCTOR*/
	{};
/*<<<<<GASTO_MEMBERS*/
	void init();
/*>>>>>GASTO_MEMBERS*/
}; // end class

/*<<<<<GASTO_POSTAMBLE*/
} // namespace gastos
} // namespace gong
#endif // GASTOS_GASTOSRECGASTO_H
/*>>>>>GASTO_POSTAMBLE*/
