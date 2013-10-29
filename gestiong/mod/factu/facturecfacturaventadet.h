/*<<<<<COPYLEFT*/
/** @file facturecfacturaventadet.h Registro de detalles de facturas de venta
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
/*<<<<<FACTURAVENTADET_PREAMBLE*/
#ifndef _FACTU_FACTURECFACTURAVENTADET_H
#define _FACTU_FACTURECFACTURAVENTADET_H
/*>>>>>FACTURAVENTADET_PREAMBLE*/

/*<<<<<FACTURAVENTADET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>FACTURAVENTADET_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<FACTURAVENTADET_CONSTRUCTOR*/
class RecFacturaVentaDet: public dbRecord
{
public:
    RecFacturaVentaDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("FACTURAVENTADET"), recid, user)
/*>>>>>FACTURAVENTADET_CONSTRUCTOR*/
    {
        addSemanticProperty( "FACTURA" );
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "DETALLE" );
    }
    /*<<<<<FACTURAVENTADET_MEMBERS*/
    virtual void beforeSaveOrDeleteRelated(dbRecord *master, bool saving); // from dbRecord
    /*>>>>>FACTURAVENTADET_MEMBERS*/
    bool actStocks( dbRecord *facturaventa, bool saving ) const;
    bool actAlbaranes( dbRecord *facturaventa, bool saving = true ) const;

    /*<<<<<FACTURAVENTADET_RELATIONS*/
    RecArticulo *getRecArticulo() const;
    empresa::RecTipoIVA *getRecTipoIVA() const;
    /*>>>>>FACTURAVENTADET_RELATIONS*/
}; // end class

/*<<<<<FACTURAVENTADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECFACTURAVENTADET_H
/*>>>>>FACTURAVENTADET_POSTAMBLE*/
