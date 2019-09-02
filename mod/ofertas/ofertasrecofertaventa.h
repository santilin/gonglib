/*<<<<<COPYLEFT*/
/** @file ofertasrecofertaventa.h Registro de ofertas de venta
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
/*<<<<<OFERTAVENTA_PREAMBLE*/
#ifndef _OFERTAS_OFERTASRECOFERTAVENTA_H
#define _OFERTAS_OFERTASRECOFERTAVENTA_H
/*>>>>>OFERTAVENTA_PREAMBLE*/

/*<<<<<OFERTAVENTA_INCLUDES*/
#include <dbappdbapplication.h>

#ifdef HAVE_TESORERIAMODULE
#include "ofertasreccuentaoferta.h"
#endif
#include "facturectipodoc.h"
#include "factureccliente.h"
#include "facturecagente.h"
#include "pagosrecformapago.h"
#include "empresarecproyecto.h"
#include "ofertasrecofertaventadet.h"
/*>>>>>OFERTAVENTA_INCLUDES*/
#include <factuiivadesglosable.h>
#include <factuitotalizablerecord.h>
#ifdef HAVE_TESORERIAMODULE
#include <tesoreriaiapuntablerecord.h>
#endif
#include "ofertasipagables.h"

namespace gong {
namespace ofertas {

/*<<<<<OFERTAVENTA_CONSTRUCTOR*/
class RecOfertaVenta: public dbRecord
,	public ofertas::IPagableOferta
,	public factu::IIVADesglosable
,	public factu::ITotalizableRecord
#ifdef HAVE_TESORERIAMODULE
	,public tesoreria::IApuntableRecord
#endif

{
public:
	RecOfertaVenta(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("OFERTAVENTA"), recid, user)
/*>>>>>OFERTAVENTA_CONSTRUCTOR*/
        , IPagableOferta( this, IPagableRecord::cobros )
        , IIVADesglosable( this, getListOfertaVentaDet() )
        , ITotalizableRecord( this, this->getListOfertaVentaDet(), venta )
#ifdef HAVE_TESORERIAMODULE
        , IApuntableRecord( this, IApuntableRecord::ABONO, "APUNTEOFERTA", "APUNTE_ID",
							"CUENTAOFERTA_ID", "CUENTAPAGO_ID", "FECHA", "ENTREGA", "NUMERO",
							"CLIENTE", false, "CLIENTE_ID", Xtring::null,
							Xtring::null, false, Xtring::null, "~CODE_AND_DESC_WITH_TABLENAME",
							"NOTAS", "PROYECTO_ID" )
#endif
    {
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "OFERTA" );
	};
    /*<<<<<OFERTAVENTA_RELATIONS*/
#ifdef HAVE_TESORERIAMODULE
	RecCuentaOferta *getRecCuentaOferta() const;
#endif
	factu::RecTipoDoc *getRecTipoDoc() const;
	factu::RecCliente *getRecCliente() const;
	factu::RecAgente *getRecAgente() const;
	pagos::RecFormaPago *getRecFormaPago() const;
	empresa::RecProyecto *getRecProyecto() const;
	RecOfertaVentaDet *getRecOfertaVentaDet( int nofertaventadet = -1 ) const;
	dbRecordList *getListOfertaVentaDet() const;
/*>>>>>OFERTAVENTA_RELATIONS*/
    /*<<<<<OFERTAVENTA_MEMBERS*/
	void init();
	virtual bool save(bool validate, bool saverelated)/* throw( dbError ) */; // from dbRecord
	virtual bool remove()/* throw( dbError ) */; // from dbRecord
	Xtring toString(int format, const Xtring &includedFields = Xtring::null) const;
/*>>>>>OFERTAVENTA_MEMBERS*/
}; // end class

/*<<<<<OFERTAVENTA_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASRECOFERTAVENTA_H
/*>>>>>OFERTAVENTA_POSTAMBLE*/

