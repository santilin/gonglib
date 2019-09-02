/*<<<<<PAGOOFERTA_PREAMBLE*/
#ifndef _OFERTAS_OFERTASRECPAGOOFERTA_H
#define _OFERTAS_OFERTASRECPAGOOFERTA_H
/*>>>>>PAGOOFERTA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file ofertasrecpagooferta.h Registro de pagos de ofertas
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

#include <pagosrecpago.h>

/*<<<<<PAGOOFERTA_INCLUDES*/
#include <dbappdbapplication.h>

#include "empresarecmoneda.h"
/*>>>>>PAGOOFERTA_INCLUDES*/

#ifdef HAVE_TESORERIAMODULE
#include <tesoreriaiapuntablerecord.h>
#include <tesoreriareccuentatesoreria.h>
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


/*<<<<<PAGOOFERTA_CONSTRUCTOR*/
class RecPagoOferta: public dbRecord
#ifdef HAVE_TESORERIAMODULE
	,public tesoreria::IApuntableRecord
#endif

{
public:
	RecPagoOferta(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PAGOOFERTA"), recid, user)
/*>>>>>PAGOOFERTA_CONSTRUCTOR*/
		, pRecFactura(0), pRecTercero(0)
#ifdef HAVE_TESORERIAMODULE
        , IApuntableRecord( this, IApuntableRecord::CARGO, "APUNTEOFERTA", "APUNTE_ID", 
							"CUENTAOFERTA_ID", "CUENTAPAGO_ID", "FECHAPAGO", 
							"IMPORTE", "DOCUMENTOPAGO", 
							"TABLATERCEROS", true, "TERCERO_ID", Xtring::null, 
							Xtring::null, false, Xtring::null, "~CODE_AND_DESC_WITH_TABLENAME",
							"NOTAS", Xtring::null )
#endif
	{};

/*<<<<<PAGOOFERTA_RELATIONS*/
	empresa::RecMoneda *getRecMoneda() const;
/*>>>>>PAGOOFERTA_RELATIONS*/
    dbRecord *getRecFactura();
    dbRecord *getRecTercero();
/*<<<<<PAGOOFERTA_MEMBERS*/
	void init();
	virtual bool save(bool validate, bool saverelated)/* throw( dbError ) */; // from dbRecord
	virtual bool remove()/* throw( dbError ) */; // from dbRecord
/*>>>>>PAGOOFERTA_MEMBERS*/
protected:
	bool actPagosFactura();
protected:
    dbRecord *pRecFactura, *pRecTercero;	
}; // end class

/*<<<<<PAGOOFERTA_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASRECPAGOOFERTA_H
/*>>>>>PAGOOFERTA_POSTAMBLE*/


