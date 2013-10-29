/*<<<<<COPYLEFT*/
/** @file facturecalbaranventadet.h Registro de detalles de albaranes de venta
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
/*<<<<<ALBARANVENTADET_PREAMBLE*/
#ifndef _FACTU_FACTURECALBARANVENTADET_H
#define _FACTU_FACTURECALBARANVENTADET_H
/*>>>>>ALBARANVENTADET_PREAMBLE*/

/*<<<<<ALBARANVENTADET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>ALBARANVENTADET_INCLUDES*/

namespace gong
{
namespace factu {

/*<<<<<ALBARANVENTADET_CONSTRUCTOR*/
class RecAlbaranVentaDet: public dbRecord
{
public:
    RecAlbaranVentaDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("ALBARANVENTADET"), recid, user)
/*>>>>>ALBARANVENTADET_CONSTRUCTOR*/
    {
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "ALBARAN" );
        addSemanticProperty( "DETALLE" );
    }
    /*<<<<<ALBARANVENTADET_RELATIONS*/
    RecArticulo *getRecArticulo() const;
    empresa::RecTipoIVA *getRecTipoIVA() const;
    /*>>>>>ALBARANVENTADET_RELATIONS*/

    /*<<<<<ALBARANVENTADET_MEMBERS*/
    virtual void beforeSaveOrDeleteRelated(dbRecord *master, bool saving); // from dbRecord
    /*>>>>>ALBARANVENTADET_MEMBERS*/
    bool actStocks( dbRecord *albaranventa, bool savingdetail ) const;
}; // end class

/*<<<<<ALBARANVENTADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECALBARANVENTADET_H
/*>>>>>ALBARANVENTADET_POSTAMBLE*/
