/*<<<<<COPYLEFT*/
/** @file factureccliente.h Registro de clientes
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
/*<<<<<CLIENTE_PREAMBLE*/
#ifndef _FACTU_FACTURECCLIENTE_H
#define _FACTU_FACTURECCLIENTE_H
/*>>>>>CLIENTE_PREAMBLE*/

/*<<<<<CLIENTE_INCLUDES*/
#include <dbappdbapplication.h>

#include "contactosreccontacto.h"
#include "pagosrecformapago.h"
#include "facturecagente.h"
/*>>>>>CLIENTE_INCLUDES*/
#include <contactosicontactablerecord.h>

namespace gong
{
namespace factu {

/*<<<<<CLIENTE_CONSTRUCTOR*/
class RecCliente: public dbRecord,
	public contactos::IContactableRecord

{
public:
	RecCliente(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("CLIENTE"), recid, user)
/*>>>>>CLIENTE_CONSTRUCTOR*/
        , contactos::IContactableRecord( this )
    {};
    /*<<<<<CLIENTE_RELATIONS*/
	contactos::RecContacto *getRecContacto() const;
	pagos::RecFormaPago *getRecFormaPago() const;
	RecAgente *getRecAgente() const;
/*>>>>>CLIENTE_RELATIONS*/
}; // end class

/*<<<<<CLIENTE_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECCLIENTE_H
/*>>>>>CLIENTE_POSTAMBLE*/
