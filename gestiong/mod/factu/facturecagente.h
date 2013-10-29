/*<<<<<COPYLEFT*/
/** @file facturecagente.h Registro de agentes comerciales
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<AGENTE_PREAMBLE*/
#ifndef _FACTU_FACTURECAGENTE_H
#define _FACTU_FACTURECAGENTE_H
/*>>>>>AGENTE_PREAMBLE*/

/*<<<<<AGENTE_INCLUDES*/
#include <dbappdbapplication.h>

#include "contactosreccontacto.h"
/*>>>>>AGENTE_INCLUDES*/

namespace gong
{
namespace factu {

/*<<<<<AGENTE_CONSTRUCTOR*/
class RecAgente: public dbRecord
{
public:
    RecAgente(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("AGENTE"), recid, user)
/*>>>>>AGENTE_CONSTRUCTOR*/
    {};
    /*<<<<<AGENTE_RELATIONS*/
    contactos::RecContacto *getRecContacto() const;
    /*>>>>>AGENTE_RELATIONS*/
}; // end class

/*<<<<<AGENTE_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECAGENTE_H
/*>>>>>AGENTE_POSTAMBLE*/


