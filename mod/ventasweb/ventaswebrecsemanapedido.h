/*<<<<<COPYLEFT*/
/** @file ventaswebrecsemanapedido.h Registro de semanas de pedidos
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
/*<<<<<SEMANAPEDIDO_PREAMBLE*/
#ifndef _VENTASWEB_VENTASWEBRECSEMANAPEDIDO_H
#define _VENTASWEB_VENTASWEBRECSEMANAPEDIDO_H
/*>>>>>SEMANAPEDIDO_PREAMBLE*/

/*<<<<<SEMANAPEDIDO_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>SEMANAPEDIDO_INCLUDES*/

namespace gong {
namespace ventasweb {

/*<<<<<SEMANAPEDIDO_CONSTRUCTOR*/
class RecSemanaPedido: public dbRecord
{
public:
	RecSemanaPedido(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("SEMANAPEDIDO"), recid, user)
/*>>>>>SEMANAPEDIDO_CONSTRUCTOR*/
    {};
}; // end class

/*<<<<<SEMANAPEDIDO_POSTAMBLE*/
} // namespace ventasweb
} // namespace gong
#endif // VENTASWEB_VENTASWEBRECSEMANAPEDIDO_H
/*>>>>>SEMANAPEDIDO_POSTAMBLE*/

