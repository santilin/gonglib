/*<<<<<COPYLEFT*/
/** @file facturecpedidoventa.h Registro de pedidos de venta
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
/*<<<<<PEDIDOVENTA_PREAMBLE*/
#ifndef _FACTU_FACTURECPEDIDOVENTA_H
#define _FACTU_FACTURECPEDIDOVENTA_H
/*>>>>>PEDIDOVENTA_PREAMBLE*/

/*<<<<<PEDIDOVENTA_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturectipodoc.h"
#include "factureccliente.h"
#include "facturecagente.h"
#include "facturecpedidoventadet.h"
/*>>>>>PEDIDOVENTA_INCLUDES*/
#include "factuiivadesglosable.h"
#include "factuitotalizablerecord.h"

namespace gong {
namespace factu {

/*<<<<<PEDIDOVENTA_CONSTRUCTOR*/
class RecPedidoVenta: public dbRecord,
	public IIVADesglosable,
	public ITotalizableRecord

{
public:
	RecPedidoVenta(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PEDIDOVENTA"), recid, user)
/*>>>>>PEDIDOVENTA_CONSTRUCTOR*/
        , IIVADesglosable( this, getListPedidoVentaDet() )
        , ITotalizableRecord( this, getListPedidoVentaDet(), venta )
    {
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "PEDIDO" );
    }
    /*<<<<<PEDIDOVENTA_MEMBERS*/
	void init();
/*>>>>>PEDIDOVENTA_MEMBERS*/
    /*<<<<<PEDIDOVENTA_RELATIONS*/
	RecTipoDoc *getRecTipoDoc() const;
	RecCliente *getRecCliente() const;
	RecAgente *getRecAgente() const;
	RecPedidoVentaDet *getRecPedidoVentaDet( int npedidoventadet = -1 ) const;
	dbRecordList *getListPedidoVentaDet() const;
/*>>>>>PEDIDOVENTA_RELATIONS*/
}; // end class

/*<<<<<PEDIDOVENTA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECPEDIDOVENTA_H
/*>>>>>PEDIDOVENTA_POSTAMBLE*/
