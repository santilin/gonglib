/*<<<<<COBROOFERTA_PREAMBLE*/
#ifndef _OFERTAS_OFERTASRECCOBROOFERTA_H
#define _OFERTAS_OFERTASRECCOBROOFERTA_H
/*>>>>>COBROOFERTA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file ofertasreccobrooferta.h Registro de cobros
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

/*<<<<<COBROOFERTA_INCLUDES*/
#include <dbappdbapplication.h>

#include "empresarecmoneda.h"
/*>>>>>COBROOFERTA_INCLUDES*/

#ifdef HAVE_TESORERIAMODULE
#include <tesoreriaiapuntablerecord.h>
typedef gong::tesoreria::RecCuentaTesoreria RecCuentaPago;
#elif defined(HAVE_CONTABMODULE)
#include <contabreccuenta.h>
#include <contabrecasiento.h>
typedef gong::contab::RecCuenta RecCuentaPago;
#else
typedef gong::dbRecord RecCuentaPago;
#endif


namespace gong {
namespace ofertas {

/*<<<<<COBROOFERTA_CONSTRUCTOR*/
class RecCobroOferta: public dbRecord
#ifdef HAVE_TESORERIAMODULE
	,public tesoreria::IApuntableRecord
#endif

{
public:
	RecCobroOferta(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("COBROOFERTA"), recid, user)
/*>>>>>COBROOFERTA_CONSTRUCTOR*/
		, pRecFactura(0), pRecTercero(0)
#ifdef HAVE_TESORERIAMODULE
        , IApuntableRecord( this, IApuntableRecord::ABONO, "APUNTEOFERTA", "APUNTE_ID", 
							"CUENTAOFERTA_ID", "CUENTAPAGO_ID", "FECHAPAGO", "IMPORTE", "DOCUMENTOPAGO", 
							"TABLATERCEROS", true, "TERCERO_ID", Xtring::null, 
							Xtring::null, false, Xtring::null, "~CODE_AND_DESC_WITH_TABLENAME",
							"NOTAS", Xtring::null )
#endif
	{};

/*<<<<<COBROOFERTA_RELATIONS*/
	empresa::RecMoneda *getRecMoneda() const;
/*>>>>>COBROOFERTA_RELATIONS*/
    dbRecord *getRecFactura();
    dbRecord *getRecTercero();
/*<<<<<COBROOFERTA_MEMBERS*/
	void init();
	virtual bool save(bool validate, bool saverelated)/* throw( dbError ) */; // from dbRecord
	virtual bool remove()/* throw( dbError ) */; // from dbRecord
/*>>>>>COBROOFERTA_MEMBERS*/
protected:
	bool actPagosFactura();
protected:
    dbRecord *pRecFactura, *pRecTercero;	
}; // class

/*<<<<<COBROOFERTA_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASRECCOBROOFERTA_H
/*>>>>>COBROOFERTA_POSTAMBLE*/

