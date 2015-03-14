/*<<<<<COPYLEFT*/
/** @file facturecfacturaventa.h Registro de facturas de venta
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
/*<<<<<FACTURAVENTA_PREAMBLE*/
#ifndef _FACTU_FACTURECFACTURAVENTA_H
#define _FACTU_FACTURECFACTURAVENTA_H
/*>>>>>FACTURAVENTA_PREAMBLE*/

#include "config.h"
/*<<<<<FACTURAVENTA_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturectipodoc.h"
#include "factureccliente.h"
#include "facturecagente.h"
#include "pagosrecformapago.h"
#include "empresarecproyecto.h"
#include "facturecfacturaventadet.h"
/*>>>>>FACTURAVENTA_INCLUDES*/
#include "factuipagables.h"
#include "factuiivadesglosable.h"
#include "factuitotalizablerecord.h"

#ifdef HAVE_CONTABMODULE
#include <contabrecasiento.h>
#include "factuiasentables.h"
#endif

#ifdef HAVE_TESORERIAMODULE
#include "tesoreriaiapuntablerecord.h"
#endif

namespace gong {
namespace factu {

/*<<<<<FACTURAVENTA_CONSTRUCTOR*/
class RecFacturaVenta: public dbRecord
,	public factu::IPagableFactura
,	public factu::IIVADesglosable
#ifdef HAVE_CONTABMODULE
	,public factu::IAsentableFactura
#endif
#ifdef HAVE_TESORERIAMODULE
	,public tesoreria::IApuntableRecord
#endif
,	public factu::ITotalizableRecord

{
public:
	RecFacturaVenta(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("FACTURAVENTA"), recid, user)
/*>>>>>FACTURAVENTA_CONSTRUCTOR*/
        , IPagableFactura( this, IPagableRecord::cobros )
        , IIVADesglosable( this, getListFacturaVentaDet() )
#ifdef HAVE_CONTABMODULE
        , IAsentableFactura( this, IAsentableFactura::facturaventa )
#endif
#ifdef HAVE_TESORERIAMODULE
        , IApuntableRecord( this, IApuntableRecord::ABONO, "APUNTETESORERIA",
							"APUNTE_ID", "CUENTAPAGO_ID", "FECHA", "ENTREGA", "NUMERO", 
							"CLIENTE", false, "CLIENTE_ID", Xtring::null, 
							Xtring::null, false, Xtring::null, "~CODE_AND_DESC_WITH_TABLENAME",
							"NOTAS", "PROYECTO_ID" )
#endif
        , ITotalizableRecord( this, getListFacturaVentaDet(), venta )
    {
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "FACTURA" );
    }
    /*<<<<<FACTURAVENTA_MEMBERS*/
	void init();
	virtual bool save(bool saverelated) throw( dbError ); // from dbRecord
	virtual bool remove() throw( dbError ); // from dbRecord
	Xtring toString(int format, const Xtring &includedFields = Xtring::null) const;
/*>>>>>FACTURAVENTA_MEMBERS*/
public:
    /*<<<<<FACTURAVENTA_RELATIONS*/
	RecTipoDoc *getRecTipoDoc() const;
	RecCliente *getRecCliente() const;
	RecAgente *getRecAgente() const;
	pagos::RecFormaPago *getRecFormaPago() const;
	empresa::RecProyecto *getRecProyecto() const;
	RecFacturaVentaDet *getRecFacturaVentaDet( int nfacturaventadet = -1 ) const;
	dbRecordList *getListFacturaVentaDet() const;
/*>>>>>FACTURAVENTA_RELATIONS*/
}; // end class

/*<<<<<FACTURAVENTA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECFACTURAVENTA_H
/*>>>>>FACTURAVENTA_POSTAMBLE*/
