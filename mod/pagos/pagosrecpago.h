/*<<<<<COPYLEFT*/
/** @file pagosrecpago.h Registro de pagos
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
/*<<<<<PAGO_PREAMBLE*/
#ifndef _PAGOS_PAGOSRECPAGO_H
#define _PAGOS_PAGOSRECPAGO_H
/*>>>>>PAGO_PREAMBLE*/

/*<<<<<PAGO_INCLUDES*/
#include <dbappdbapplication.h>

#include "empresarecmoneda.h"
/*>>>>>PAGO_INCLUDES*/

#ifdef HAVE_CONTABMODULE
#include <contabreccuenta.h>
#include <contabrecasiento.h>
namespace gong {
namespace contab {
typedef RecCuenta RecCuentaPago;
}
}
#else
typedef gong::dbRecord RecCuentaPago;
#endif

namespace gong {
namespace pagos {

/*<<<<<PAGO_CONSTRUCTOR*/
class RecPago: public dbRecord
{
public:
	RecPago(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PAGO"), recid, user)
/*>>>>>PAGO_CONSTRUCTOR*/
        , pRecFactura(0), pRecTercero(0)
    { };
    RecPago(dbTableDefinition *tbldef, dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, tbldef, recid, user)
        , pRecFactura(0), pRecTercero(0)
    { };
    /*<<<<<PAGO_RELATIONS*/
	empresa::RecMoneda *getRecMoneda() const;
/*>>>>>PAGO_RELATIONS*/
    bool actPagosFactura();
#ifdef HAVE_CONTABMODULE
    contab::RecCuentaPago *getRecCuentaPago() const;
#endif
    dbRecord *getRecFactura();
    dbRecord *getRecTercero();

    /*<<<<<PAGO_MEMBERS*/
	void init();
	virtual bool save(bool saverelated) throw( dbError ); // from dbRecord
	virtual bool remove() throw( dbError ); // from dbRecord
/*>>>>>PAGO_MEMBERS*/
protected:
    dbRecord *pRecFactura, *pRecTercero;
}; // class

/*<<<<<PAGO_POSTAMBLE*/
} // namespace pagos
} // namespace gong
#endif // PAGOS_PAGOSRECPAGO_H
/*>>>>>PAGO_POSTAMBLE*/

