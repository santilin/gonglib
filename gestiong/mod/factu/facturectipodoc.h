/*<<<<<COPYLEFT*/
/** @file facturectipodoc.h Registro de tipos de facturas
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
/*<<<<<TIPODOC_PREAMBLE*/
#ifndef _FACTU_FACTURECTIPODOC_H
#define _FACTU_FACTURECTIPODOC_H
/*>>>>>TIPODOC_PREAMBLE*/

#include "config.h"
/*<<<<<TIPODOC_INCLUDES*/
#include <dbappdbapplication.h>

#include "pagosrecformapago.h"
#include "empresarecproyecto.h"
#include "facturecagente.h"
/*>>>>>TIPODOC_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<TIPODOC_CONSTRUCTOR*/
class RecTipoDoc: public dbRecord
{
public:
    RecTipoDoc(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("TIPODOC"), recid, user)
/*>>>>>TIPODOC_CONSTRUCTOR*/
    {};
    bool generaAsientos() const;
    static Xtring formatNumDocumento(int codempresa, int ejercicio,
                                     int numero, int codagente, const Xtring& formato);

    /*<<<<<TIPODOC_RELATIONS*/
    pagos::RecFormaPago *getRecFormaPago() const;
    empresa::RecProyecto *getRecProyecto() const;
    RecAgente *getRecAgente() const;
    /*>>>>>TIPODOC_RELATIONS*/
}; // end class

/*<<<<<TIPODOC_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECTIPODOC_H
/*>>>>>TIPODOC_POSTAMBLE*/

