/*<<<<<COPYLEFT*/
/** @file gastostiporecgastotipo.h Registro de definición de gastos e ingresos
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
/*<<<<<GASTOTIPO_PREAMBLE*/
#ifndef _GASTOSTIPO_GASTOSTIPORECGASTOTIPO_H
#define _GASTOSTIPO_GASTOSTIPORECGASTOTIPO_H
/*>>>>>GASTOTIPO_PREAMBLE*/

/*<<<<<GASTOTIPO_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>GASTOTIPO_INCLUDES*/

namespace gong {
namespace gastostipo {

/*<<<<<GASTOTIPO_CONSTRUCTOR*/
class RecGastoTipo: public dbRecord
{
public:
	RecGastoTipo(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("GASTOTIPO"), recid, user)
/*>>>>>GASTOTIPO_CONSTRUCTOR*/
	{};
}; // end class

/*<<<<<GASTOTIPO_POSTAMBLE*/
} // namespace gastostipo
} // namespace gong
#endif // GASTOSTIPO_GASTOSTIPORECGASTOTIPO_H
/*>>>>>GASTOTIPO_POSTAMBLE*/
