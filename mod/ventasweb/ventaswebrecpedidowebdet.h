/*<<<<<COPYLEFT*/
/** @file ventaswebrecpedidowebdet.h Registro de detalles de los pedidos web
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
/*<<<<<PEDIDOWEBDET_PREAMBLE*/
#ifndef _VENTASWEB_VENTASWEBRECPEDIDOWEBDET_H
#define _VENTASWEB_VENTASWEBRECPEDIDOWEBDET_H
/*>>>>>PEDIDOWEBDET_PREAMBLE*/

/*<<<<<PEDIDOWEBDET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>PEDIDOWEBDET_INCLUDES*/

namespace gong {
namespace ventasweb {

/*<<<<<PEDIDOWEBDET_CONSTRUCTOR*/
class RecPedidoWebDet: public dbRecord
{
public:
	RecPedidoWebDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PEDIDOWEBDET"), recid, user)
/*>>>>>PEDIDOWEBDET_CONSTRUCTOR*/
    {
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "PEDIDO" );
        addSemanticProperty( "DETALLE" );
    };
    /*<<<<<PEDIDOWEBDET_RELATIONS*/
	factu::RecArticulo *getRecArticulo() const;
	empresa::RecTipoIVA *getRecTipoIVA() const;
/*>>>>>PEDIDOWEBDET_RELATIONS*/
}; // end class

/*<<<<<PEDIDOWEBDET_POSTAMBLE*/
} // namespace ventasweb
} // namespace gong
#endif // VENTASWEB_VENTASWEBRECPEDIDOWEBDET_H
/*>>>>>PEDIDOWEBDET_POSTAMBLE*/

