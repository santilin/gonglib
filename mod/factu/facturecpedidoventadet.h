/*<<<<<COPYLEFT*/
/** @file facturecpedidoventadet.h Registro de detalles de pedidos de venta
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
/*<<<<<PEDIDOVENTADET_PREAMBLE*/
#ifndef _FACTU_FACTURECPEDIDOVENTADET_H
#define _FACTU_FACTURECPEDIDOVENTADET_H
/*>>>>>PEDIDOVENTADET_PREAMBLE*/

/*<<<<<PEDIDOVENTADET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>PEDIDOVENTADET_INCLUDES*/

namespace gong {
namespace factu {

class RecArticulo;

/*<<<<<PEDIDOVENTADET_CONSTRUCTOR*/
class RecPedidoVentaDet: public dbRecord
{
public:
    RecPedidoVentaDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PEDIDOVENTADET"), recid, user)
/*>>>>>PEDIDOVENTADET_CONSTRUCTOR*/
    {
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "PEDIDO" );
        addSemanticProperty( "DETALLE" );
    }
    /*<<<<<PEDIDOVENTADET_RELATIONS*/
    RecArticulo *getRecArticulo() const;
    empresa::RecTipoIVA *getRecTipoIVA() const;
    /*>>>>>PEDIDOVENTADET_RELATIONS*/
}; // end class

/*<<<<<PEDIDOVENTADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECPEDIDOVENTADET_H
/*>>>>>PEDIDOVENTADET_POSTAMBLE*/

