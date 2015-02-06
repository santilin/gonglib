/*<<<<<COPYLEFT*/
/** @file pagosreccobro.h Registro de cobros
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
/*<<<<<COBRO_PREAMBLE*/
#ifndef _PAGOS_PAGOSRECCOBRO_H
#define _PAGOS_PAGOSRECCOBRO_H
/*>>>>>COBRO_PREAMBLE*/

#include "config.h"
/*<<<<<COBRO_INCLUDES*/
#include <dbappdbapplication.h>

#include "empresarecmoneda.h"
#include "pagosrecremesacobro.h"
/*>>>>>COBRO_INCLUDES*/

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
namespace pagos {

/*<<<<<COBRO_CONSTRUCTOR*/
class RecCobro: public dbRecord
#ifdef HAVE_TESORERIAMODULE
	,public tesoreria::IApuntableRecord
#endif

{
public:
	RecCobro(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("COBRO"), recid, user)
/*>>>>>COBRO_CONSTRUCTOR*/
        , pRecFactura(0), pRecTercero(0)
#ifdef HAVE_TESORERIAMODULE
        , IApuntableRecord( this, IApuntableRecord::ABONO, "APUNTE_ID", "CUENTAPAGO_ID", "FECHAPAGO", "IMPORTE", "DOCUMENTOPAGO", 
							"TABLATERCEROS", true, "TERCERO_ID", Xtring::null, 
							Xtring::null, false, Xtring::null, "~CODE_AND_DESC_WITH_TABLENAME",
							"NOTAS", Xtring::null )
#endif
    {
	};
    RecCobro(dbTableDefinition *tbldef, dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, tbldef, recid, user)
        , pRecFactura(0), pRecTercero(0)
#ifdef HAVE_TESORERIAMODULE
        , IApuntableRecord( this, IApuntableRecord::ABONO, "APUNTE_ID", "CUENTAPAGO_ID", "FECHAPAGO", "IMPORTE", "DOCUMENTOPAGO", 
							"TABLATERCEROS", true, "TERCERO_ID", Xtring::null, 
							Xtring::null, false, Xtring::null, "~CODE_AND_DESC_WITH_TABLENAME",
							"NOTAS", Xtring::null )
#endif
    {
	};
/*<<<<<COBRO_RELATIONS*/
	empresa::RecMoneda *getRecMoneda() const;
	RecRemesaCobro *getRecRemesaCobro() const;
/*>>>>>COBRO_RELATIONS*/
    bool actCobrosFactura();
#if defined(HAVE_CONTABMODULE) || defined(HAVE_TESORERIAMODULE)
    RecCuentaPago *getRecCuentaPago() const;
#endif
    dbRecord *getRecFactura();
    dbRecord *getRecTercero();
    Xtring formatNumRecibo(int codempresa, int ejercicio, const Xtring& formato);

    /*<<<<<COBRO_MEMBERS*/
	void init();
	virtual bool save(bool saverelated) throw( dbError ); // from dbRecord
	virtual bool remove() throw( dbError ); // from dbRecord
/*>>>>>COBRO_MEMBERS*/
protected:
    dbRecord *pRecFactura, *pRecTercero;
}; // class

/*<<<<<COBRO_POSTAMBLE*/
} // namespace pagos
} // namespace gong
#endif // PAGOS_PAGOSRECCOBRO_H
/*>>>>>COBRO_POSTAMBLE*/

