/*<<<<<COPYLEFT*/
/** @file facturecpedidocompra.h Registro de pedidos de compra
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
/*<<<<<PEDIDOCOMPRA_PREAMBLE*/
#ifndef _FACTU_FACTURECPEDIDOCOMPRA_H
#define _FACTU_FACTURECPEDIDOCOMPRA_H
/*>>>>>PEDIDOCOMPRA_PREAMBLE*/

/*<<<<<PEDIDOCOMPRA_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturectipodoc.h"
#include "facturecproveedora.h"
#include "facturecagente.h"
#include "facturecpedidocompradet.h"
/*>>>>>PEDIDOCOMPRA_INCLUDES*/
#include "factuitotalizablerecord.h"

namespace gong {
namespace factu {

/*<<<<<PEDIDOCOMPRA_CONSTRUCTOR*/
class RecPedidoCompra: public dbRecord
,	public ITotalizableRecord

{
public:
	RecPedidoCompra(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PEDIDOCOMPRA"), recid, user)
/*>>>>>PEDIDOCOMPRA_CONSTRUCTOR*/
        , ITotalizableRecord( this, getListPedidoCompraDet(), compra )
    {
        addSemanticProperty( "COMPRA" );
        addSemanticProperty( "PEDIDO" );
    }
    /*<<<<<PEDIDOCOMPRA_MEMBERS*/
	void init();
/*>>>>>PEDIDOCOMPRA_MEMBERS*/
    /*<<<<<PEDIDOCOMPRA_RELATIONS*/
	RecTipoDoc *getRecTipoDoc() const;
	RecProveedora *getRecProveedora() const;
	RecAgente *getRecAgente() const;
	RecPedidoCompraDet *getRecPedidoCompraDet( int npedidocompradet = -1 ) const;
	dbRecordList *getListPedidoCompraDet() const;
/*>>>>>PEDIDOCOMPRA_RELATIONS*/
}; // end class

/*<<<<<PEDIDOCOMPRA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECPEDIDOCOMPRA_H
/*>>>>>PEDIDOCOMPRA_POSTAMBLE*/
