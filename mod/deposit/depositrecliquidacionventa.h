/*<<<<<COPYLEFT*/
/** @file depositrecliquidacionventa.h Registro de pedidos de venta
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
/*<<<<<LIQUIDACIONVENTA_PREAMBLE*/
#ifndef _DEPOSIT_DEPOSITRECLIQUIDACIONVENTA_H
#define _DEPOSIT_DEPOSITRECLIQUIDACIONVENTA_H
/*>>>>>LIQUIDACIONVENTA_PREAMBLE*/

/*<<<<<LIQUIDACIONVENTA_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturectipodoc.h"
#include "factureccliente.h"
#include "facturecagente.h"
#include "depositrecliquidacionventadet.h"
/*>>>>>LIQUIDACIONVENTA_INCLUDES*/
#include <factuitotalizablerecord.h>

namespace gong {
namespace deposit {

/*<<<<<LIQUIDACIONVENTA_CONSTRUCTOR*/
class RecLiquidacionVenta: public dbRecord
,	public factu::ITotalizableRecord

{
public:
	RecLiquidacionVenta(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("LIQUIDACIONVENTA"), recid, user)
/*>>>>>LIQUIDACIONVENTA_CONSTRUCTOR*/
		, factu::ITotalizableRecord( this, getListLiquidacionVentaDet(), venta )
	{
		addSemanticProperty( "VENTA" );
		addSemanticProperty( "LIQUIDACION" );
	}

/*<<<<<LIQUIDACIONVENTA_MEMBERS*/
	void init();
	virtual bool save(bool validate, bool saverelated) throw( dbError ); // from dbRecord
/*>>>>>LIQUIDACIONVENTA_MEMBERS*/
/*<<<<<LIQUIDACIONVENTA_RELATIONS*/
	factu::RecTipoDoc *getRecTipoDoc() const;
	factu::RecCliente *getRecCliente() const;
	factu::RecAgente *getRecAgente() const;
	RecLiquidacionVentaDet *getRecLiquidacionVentaDet( int nliquidacionventadet = -1 ) const;
	dbRecordList *getListLiquidacionVentaDet() const;
/*>>>>>LIQUIDACIONVENTA_RELATIONS*/
}; // end class

/*<<<<<LIQUIDACIONVENTA_POSTAMBLE*/
} // namespace deposit
} // namespace gong
#endif // DEPOSIT_DEPOSITRECLIQUIDACIONVENTA_H
/*>>>>>LIQUIDACIONVENTA_POSTAMBLE*/


