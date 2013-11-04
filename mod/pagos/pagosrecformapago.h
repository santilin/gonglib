/*<<<<<COPYLEFT*/
/** @file pagosrecformapago.h Registro de formas de pago
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
/*<<<<<FORMAPAGO_PREAMBLE*/
#ifndef _PAGOS_PAGOSRECFORMAPAGO_H
#define _PAGOS_PAGOSRECFORMAPAGO_H
/*>>>>>FORMAPAGO_PREAMBLE*/

/*<<<<<FORMAPAGO_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>FORMAPAGO_INCLUDES*/

namespace gong {
namespace pagos {

/*<<<<<FORMAPAGO_CONSTRUCTOR*/
class RecFormaPago: public dbRecord
{
public:
	RecFormaPago(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("FORMAPAGO"), recid, user)
/*>>>>>FORMAPAGO_CONSTRUCTOR*/
    {}
    enum Tipo { Contado = 1, GeneraRecibos = 2, Pendiente = 3, SeIgnora = 4 };

}; // class
/*<<<<<FORMAPAGO_POSTAMBLE*/
} // namespace pagos
} // namespace gong
#endif // PAGOS_PAGOSRECFORMAPAGO_H
/*>>>>>FORMAPAGO_POSTAMBLE*/
