/*<<<<<COPYLEFT*/
/** @file ventaswebrecpedidoweb.h Registro de pedidos web
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
/*<<<<<PEDIDOWEB_PREAMBLE*/
#ifndef _VENTASWEB_VENTASWEBRECPEDIDOWEB_H
#define _VENTASWEB_VENTASWEBRECPEDIDOWEB_H
/*>>>>>PEDIDOWEB_PREAMBLE*/

/*<<<<<PEDIDOWEB_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecagente.h"
#include "ventaswebrecpedidowebdet.h"
/*>>>>>PEDIDOWEB_INCLUDES*/
#include <factuitotalizablerecord.h>

namespace gong {
namespace ventasweb {

/*<<<<<PEDIDOWEB_CONSTRUCTOR*/
class RecPedidoWeb: public dbRecord
,	public factu::ITotalizableRecord

{
public:
	RecPedidoWeb(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PEDIDOWEB"), recid, user)
/*>>>>>PEDIDOWEB_CONSTRUCTOR*/
        , factu::ITotalizableRecord( this, getListPedidoWebDet(), venta )
    {
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "PEDIDO" );
    }
    /*<<<<<PEDIDOWEB_RELATIONS*/
	factu::RecAgente *getRecAgente() const;
	RecPedidoWebDet *getRecPedidoWebDet( int npedidowebdet = -1 ) const;
	dbRecordList *getListPedidoWebDet() const;
/*>>>>>PEDIDOWEB_RELATIONS*/
    /*<<<<<PEDIDOWEB_MEMBERS*/
	void init();
/*>>>>>PEDIDOWEB_MEMBERS*/
}; // end class

/*<<<<<PEDIDOWEB_POSTAMBLE*/
} // namespace ventasweb
} // namespace gong
#endif // VENTASWEB_VENTASWEBRECPEDIDOWEB_H
/*>>>>>PEDIDOWEB_POSTAMBLE*/
