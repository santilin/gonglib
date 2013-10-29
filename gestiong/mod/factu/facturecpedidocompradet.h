/*<<<<<COPYLEFT*/
/** @file facturecpedidocompradet.h Registro de detalles de pedidos de compra
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
/*<<<<<PEDIDOCOMPRADET_PREAMBLE*/
#ifndef _FACTU_FACTURECPEDIDOCOMPRADET_H
#define _FACTU_FACTURECPEDIDOCOMPRADET_H
/*>>>>>PEDIDOCOMPRADET_PREAMBLE*/

/*<<<<<PEDIDOCOMPRADET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>PEDIDOCOMPRADET_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<PEDIDOCOMPRADET_CONSTRUCTOR*/
class RecPedidoCompraDet: public dbRecord
{
public:
    RecPedidoCompraDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PEDIDOCOMPRADET"), recid, user)
/*>>>>>PEDIDOCOMPRADET_CONSTRUCTOR*/
    {
        addSemanticProperty( "COMPRA" );
        addSemanticProperty( "PEDIDO" );
        addSemanticProperty( "DETALLE" );
    }
    /*<<<<<PEDIDOCOMPRADET_RELATIONS*/
    RecArticulo *getRecArticulo() const;
    empresa::RecTipoIVA *getRecTipoIVA() const;
    /*>>>>>PEDIDOCOMPRADET_RELATIONS*/
}; // end class

/*<<<<<PEDIDOCOMPRADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECPEDIDOCOMPRADET_H
/*>>>>>PEDIDOCOMPRADET_POSTAMBLE*/

