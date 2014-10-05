/*<<<<<COPYLEFT*/
/** @file facturecalbarancompra.h Registro de albaranes de compra
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
/*<<<<<ALBARANCOMPRA_PREAMBLE*/
#ifndef _FACTU_FACTURECALBARANCOMPRA_H
#define _FACTU_FACTURECALBARANCOMPRA_H
/*>>>>>ALBARANCOMPRA_PREAMBLE*/

#include "config.h"
/*<<<<<ALBARANCOMPRA_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturectipodoc.h"
#include "facturecproveedora.h"
#include "facturecagente.h"
#include "pagosrecformapago.h"
#include "empresarecproyecto.h"
#include "facturecalbarancompradet.h"
/*>>>>>ALBARANCOMPRA_INCLUDES*/
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

/*<<<<<ALBARANCOMPRA_CONSTRUCTOR*/
class RecAlbaranCompra: public dbRecord,
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
	RecAlbaranCompra(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("ALBARANCOMPRA"), recid, user)
/*>>>>>ALBARANCOMPRA_CONSTRUCTOR*/
        , IPagableAlbaran( this, IPagableRecord::pagos )
        , IIVADesglosable( this, getListAlbaranCompraDet() )
#ifdef HAVE_CONTABMODULE
        , IAsentableFactura( this, IAsentableFactura::albarancompra )
#endif
#ifdef HAVE_TESORERIAMODULE
        , IApuntableRecord( this, "APUNTE_ID", "CUENTAPAGO_ID", "FECHA", "ENTREGA", "NUMERO", 
							"PROVEEDORA", "PROVEEDORA_ID", Xtring::null, 
							Xtring::null, Xtring::null, Xtring::null,
							"NOTAS", "PROYECTO_ID" )
#endif
        , ITotalizableRecord( this, this->getListAlbaranCompraDet(), compra )
    {
        addSemanticProperty( "COMPRA" );
        addSemanticProperty( "ALBARAN" );
    }
    /*<<<<<ALBARANCOMPRA_MEMBERS*/
	void init();
	virtual bool save(bool saverelated) throw( dbError ); // from dbRecord
	virtual bool remove() throw( dbError ); // from dbRecord
	Xtring toString(int format, const Xtring &includedFields = Xtring::null) const;
/*>>>>>ALBARANCOMPRA_MEMBERS*/
    /*<<<<<ALBARANCOMPRA_RELATIONS*/
	RecTipoDoc *getRecTipoDoc() const;
	RecProveedora *getRecProveedora() const;
	RecAgente *getRecAgente() const;
	pagos::RecFormaPago *getRecFormaPago() const;
	empresa::RecProyecto *getRecProyecto() const;
	RecAlbaranCompraDet *getRecAlbaranCompraDet( int nalbarancompradet = -1 ) const;
	dbRecordList *getListAlbaranCompraDet() const;
/*>>>>>ALBARANCOMPRA_RELATIONS*/
}; // end class

/*<<<<<ALBARANCOMPRA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECALBARANCOMPRA_H
/*>>>>>ALBARANCOMPRA_POSTAMBLE*/
