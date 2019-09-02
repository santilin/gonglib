/*<<<<<COPYLEFT*/
/** @file ofertasrecofertacompra.h Registro de ofertas de compra
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
/*<<<<<OFERTACOMPRA_PREAMBLE*/
#ifndef _OFERTAS_OFERTASRECOFERTACOMPRA_H
#define _OFERTAS_OFERTASRECOFERTACOMPRA_H
/*>>>>>OFERTACOMPRA_PREAMBLE*/

/*<<<<<OFERTACOMPRA_INCLUDES*/
#include <dbappdbapplication.h>

#ifdef HAVE_TESORERIAMODULE
#include "ofertasreccuentaoferta.h"
#endif
#include "facturectipodoc.h"
#include "facturecproveedora.h"
#include "facturecagente.h"
#include "pagosrecformapago.h"
#include "empresarecproyecto.h"
#include "ofertasrecofertacompradet.h"
/*>>>>>OFERTACOMPRA_INCLUDES*/
#include <factuiivadesglosable.h>
#include <factuitotalizablerecord.h>
#include <pagosipagablerecord.h>
#include "ofertasipagables.h"

#ifdef HAVE_CONTABMODULE
#include <contabrecasiento.h>
#include "factuiasentables.h"
#endif

#ifdef HAVE_TESORERIAMODULE
#include "tesoreriaiapuntablerecord.h"
#endif

namespace gong {
namespace ofertas {

/*<<<<<OFERTACOMPRA_CONSTRUCTOR*/
class RecOfertaCompra: public dbRecord
,	public ofertas::IPagableOferta
,	public factu::IIVADesglosable
,	public factu::ITotalizableRecord
#ifdef HAVE_TESORERIAMODULE
	,public tesoreria::IApuntableRecord
#endif

{
public:
	RecOfertaCompra(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("OFERTACOMPRA"), recid, user)
/*>>>>>OFERTACOMPRA_CONSTRUCTOR*/
        , IPagableOferta( this, IPagableRecord::pagos )
        , IIVADesglosable( this, getListOfertaCompraDet() )
        , ITotalizableRecord( this, this->getListOfertaCompraDet(), compra )
#ifdef HAVE_TESORERIAMODULE
        , IApuntableRecord( this, IApuntableRecord::CARGO, "APUNTEOFERTA", "APUNTE_ID", 
							"CUENTAOFERTA_ID", "CUENTAPAGO_ID", "FECHA", "ENTREGA", "NUMERO", 
							"PROVEEDORA", false, "PROVEEDORA_ID", Xtring::null, 
							Xtring::null, false, Xtring::null, "~CODE_AND_DESC_WITH_TABLENAME",
							"NOTAS", "PROYECTO_ID" )
#endif
    {
        addSemanticProperty( "COMPRA" );
        addSemanticProperty( "OFERTA" );
	};

    /*<<<<<OFERTACOMPRA_RELATIONS*/
#ifdef HAVE_TESORERIAMODULE
	RecCuentaOferta *getRecCuentaOferta() const;
#endif
	factu::RecTipoDoc *getRecTipoDoc() const;
	factu::RecProveedora *getRecProveedora() const;
	factu::RecAgente *getRecAgente() const;
	pagos::RecFormaPago *getRecFormaPago() const;
	empresa::RecProyecto *getRecProyecto() const;
	RecOfertaCompraDet *getRecOfertaCompraDet( int nofertacompradet = -1 ) const;
	dbRecordList *getListOfertaCompraDet() const;
/*>>>>>OFERTACOMPRA_RELATIONS*/

/*<<<<<OFERTACOMPRA_MEMBERS*/
	void init();
	virtual bool save(bool validate, bool saverelated)/* throw( dbError ) */; // from dbRecord
	virtual bool remove()/* throw( dbError ) */; // from dbRecord
	Xtring toString(int format, const Xtring &includedFields = Xtring::null) const;
/*>>>>>OFERTACOMPRA_MEMBERS*/

}; // end class

/*<<<<<OFERTACOMPRA_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASRECOFERTACOMPRA_H
/*>>>>>OFERTACOMPRA_POSTAMBLE*/
