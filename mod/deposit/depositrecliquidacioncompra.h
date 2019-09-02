/*<<<<<COPYLEFT*/
/** @file depositrecliquidacioncompra.h Registro de pedidos de compra
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
/*<<<<<LIQUIDACIONCOMPRA_PREAMBLE*/
#ifndef _DEPOSIT_DEPOSITRECLIQUIDACIONCOMPRA_H
#define _DEPOSIT_DEPOSITRECLIQUIDACIONCOMPRA_H
/*>>>>>LIQUIDACIONCOMPRA_PREAMBLE*/

/*<<<<<LIQUIDACIONCOMPRA_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturectipodoc.h"
#include "facturecproveedora.h"
#include "depositrecliquidacioncompradet.h"
/*>>>>>LIQUIDACIONCOMPRA_INCLUDES*/
#include <factuitotalizablerecord.h>

namespace gong {
namespace deposit {

/*<<<<<LIQUIDACIONCOMPRA_CONSTRUCTOR*/
class RecLiquidacionCompra: public dbRecord
,	public factu::ITotalizableRecord

{
public:
	RecLiquidacionCompra(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("LIQUIDACIONCOMPRA"), recid, user)
/*>>>>>LIQUIDACIONCOMPRA_CONSTRUCTOR*/
		, factu::ITotalizableRecord( this, getListLiquidacionCompraDet(), compra )
    {
		addSemanticProperty( "COMPRA" );
		addSemanticProperty( "LIQUIDACION" );
	};

/*<<<<<LIQUIDACIONCOMPRA_MEMBERS*/
	void init();
	virtual bool remove() throw( dbError ); // from dbRecord
/*>>>>>LIQUIDACIONCOMPRA_MEMBERS*/
/*<<<<<LIQUIDACIONCOMPRA_RELATIONS*/
	factu::RecTipoDoc *getRecTipoDoc() const;
	factu::RecProveedora *getRecProveedora() const;
	RecLiquidacionCompraDet *getRecLiquidacionCompraDet( int nliquidacioncompradet = -1 ) const;
	dbRecordList *getListLiquidacionCompraDet() const;
/*>>>>>LIQUIDACIONCOMPRA_RELATIONS*/

}; // end class

/*<<<<<LIQUIDACIONCOMPRA_POSTAMBLE*/
} // namespace deposit
} // namespace gong
#endif // DEPOSIT_DEPOSITRECLIQUIDACIONCOMPRA_H
/*>>>>>LIQUIDACIONCOMPRA_POSTAMBLE*/

