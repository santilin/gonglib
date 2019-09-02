/*<<<<<CUENTAOFERTA_PREAMBLE*/
#ifndef _OFERTAS_OFERTASRECCUENTAOFERTA_H
#define _OFERTAS_OFERTASRECCUENTAOFERTA_H
/*>>>>>CUENTAOFERTA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file ofertasreccuentaoferta.h Registro de cuentas de ofertas
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

/*<<<<<CUENTAOFERTA_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>CUENTAOFERTA_INCLUDES*/

namespace gong {
namespace ofertas {

/*<<<<<CUENTAOFERTA_CONSTRUCTOR*/
class RecCuentaOferta: public dbRecord
{
public:
	RecCuentaOferta(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("CUENTAOFERTA"), recid, user)
/*>>>>>CUENTAOFERTA_CONSTRUCTOR*/
    {};

/*<<<<<CUENTAOFERTA_MEMBERS*/
	void init();
/*>>>>>CUENTAOFERTA_MEMBERS*/

}; // class

/*<<<<<CUENTAOFERTA_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASRECCUENTAOFERTA_H
/*>>>>>CUENTAOFERTA_POSTAMBLE*/

