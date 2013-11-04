/*<<<<<COPYLEFT*/
/** @file facturecproveedora.h Registro de proveedoras
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
/*<<<<<PROVEEDORA_PREAMBLE*/
#ifndef _FACTU_FACTURECPROVEEDORA_H
#define _FACTU_FACTURECPROVEEDORA_H
/*>>>>>PROVEEDORA_PREAMBLE*/

/*<<<<<PROVEEDORA_INCLUDES*/
#include <dbappdbapplication.h>

#include "contactosreccontacto.h"
#include "facturecagente.h"
#include "pagosrecformapago.h"
/*>>>>>PROVEEDORA_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<PROVEEDORA_CONSTRUCTOR*/
class RecProveedora: public dbRecord
{
public:
	RecProveedora(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PROVEEDORA"), recid, user)
/*>>>>>PROVEEDORA_CONSTRUCTOR*/
    {};

    /*<<<<<PROVEEDORA_RELATIONS*/
	contactos::RecContacto *getRecContacto() const;
	RecAgente *getRecAgente() const;
	pagos::RecFormaPago *getRecFormaPago() const;
/*>>>>>PROVEEDORA_RELATIONS*/
}; // end class

/*<<<<<PROVEEDORA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECPROVEEDORA_H
/*>>>>>PROVEEDORA_POSTAMBLE*/
