/*<<<<<COPYLEFT*/
/** @file facturecalbaranventa.h Registro de albaranes de venta
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
/*<<<<<ALBARANVENTA_PREAMBLE*/
#ifndef _FACTU_FACTURECALBARANVENTA_H
#define _FACTU_FACTURECALBARANVENTA_H
/*>>>>>ALBARANVENTA_PREAMBLE*/

#include "config.h"
/*<<<<<ALBARANVENTA_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturectipodoc.h"
#include "factureccliente.h"
#include "facturecagente.h"
#include "pagosrecformapago.h"
#include "empresarecproyecto.h"
#include "facturecalbaranventadet.h"
/*>>>>>ALBARANVENTA_INCLUDES*/
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

/*<<<<<ALBARANVENTA_CONSTRUCTOR*/
class RecAlbaranVenta: public dbRecord,
	public factu::IPagableAlbaran,
	public factu::IIVADesglosable,
#ifdef HAVE_CONTABMODULE
	public factu::IAsentableFactura,
#endif
#ifdef HAVE_TESORERIAMODULE
	public tesoreria::IApuntableRecord,
#endif
	public factu::ITotalizableRecord

{
public:
	RecAlbaranVenta(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("ALBARANVENTA"), recid, user)
/*>>>>>ALBARANVENTA_CONSTRUCTOR*/
        , IPagableAlbaran( this, IPagableRecord::cobros )
        , IIVADesglosable( this, getListAlbaranVentaDet() )
#ifdef HAVE_CONTABMODULE
        , IAsentableFactura( this, IAsentableFactura::albaranventa )
#endif
#ifdef HAVE_TESORERIAMODULE
        , IApuntableRecord( this, IApuntableRecord::ABONO, "APUNTE_ID", "CUENTAPAGO_ID", "FECHA", "ENTREGA", "NUMERO", 
							"CLIENTE", false, "CLIENTE_ID", Xtring::null, 
							Xtring::null, false, Xtring::null, "~CODE_AND_DESC_WITH_TABLENAME",
							"NOTAS", "PROYECTO_ID" )
#endif        
		, ITotalizableRecord( this, getListAlbaranVentaDet(), venta )
    {
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "ALBARAN" );
    }
    void setNextNumero( bool checkdup );

    /*<<<<<ALBARANVENTA_MEMBERS*/
	void init();
	virtual bool save(bool saverelated) throw( dbError ); // from dbRecord
	virtual bool remove() throw( dbError ); // from dbRecord
	Xtring toString(int format, const Xtring &includedFields = Xtring::null) const;
/*>>>>>ALBARANVENTA_MEMBERS*/
    /*<<<<<ALBARANVENTA_RELATIONS*/
	RecTipoDoc *getRecTipoDoc() const;
	RecCliente *getRecCliente() const;
	RecAgente *getRecAgente() const;
	pagos::RecFormaPago *getRecFormaPago() const;
	empresa::RecProyecto *getRecProyecto() const;
	RecAlbaranVentaDet *getRecAlbaranVentaDet( int nalbaranventadet = -1 ) const;
	dbRecordList *getListAlbaranVentaDet() const;
/*>>>>>ALBARANVENTA_RELATIONS*/
}; // end class

/*<<<<<ALBARANVENTA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECALBARANVENTA_H
/*>>>>>ALBARANVENTA_POSTAMBLE*/
