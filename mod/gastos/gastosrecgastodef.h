/*<<<<<COPYLEFT*/
/** @file gastosrecgastodef.h Registro de definición de gastos e ingresos
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
/*<<<<<GASTODEF_PREAMBLE*/
#ifndef _GASTOS_GASTOSRECGASTODEF_H
#define _GASTOS_GASTOSRECGASTODEF_H
/*>>>>>GASTODEF_PREAMBLE*/

/*<<<<<GASTODEF_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>GASTODEF_INCLUDES*/

namespace gong {
namespace gastos {

/*<<<<<GASTODEF_CONSTRUCTOR*/
class RecGastoDef: public dbRecord
{
public:
	RecGastoDef(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("GASTODEF"), recid, user)
/*>>>>>GASTODEF_CONSTRUCTOR*/
	{};
}; // end class

/*<<<<<GASTODEF_POSTAMBLE*/
} // namespace gastos
} // namespace gong
#endif // GASTOS_GASTOSRECGASTODEF_H
/*>>>>>GASTODEF_POSTAMBLE*/
