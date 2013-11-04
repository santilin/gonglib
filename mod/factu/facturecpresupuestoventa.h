/*<<<<<COPYLEFT*/
/** @file facturecpresupuestoventa.h Registro de presupuestos de venta
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
/*<<<<<PRESUPUESTOVENTA_PREAMBLE*/
#ifndef _FACTU_FACTURECPRESUPUESTOVENTA_H
#define _FACTU_FACTURECPRESUPUESTOVENTA_H
/*>>>>>PRESUPUESTOVENTA_PREAMBLE*/

/*<<<<<PRESUPUESTOVENTA_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturectipodoc.h"
#include "factureccliente.h"
#include "facturecagente.h"
#include "facturecpresupuestoventadet.h"
/*>>>>>PRESUPUESTOVENTA_INCLUDES*/
#include "factuiivadesglosable.h"
#include "factuitotalizablerecord.h"

namespace gong {
namespace factu {

/*<<<<<PRESUPUESTOVENTA_CONSTRUCTOR*/
class RecPresupuestoVenta: public dbRecord,
	public IIVADesglosable,
	public ITotalizableRecord

{
public:
	RecPresupuestoVenta(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PRESUPUESTOVENTA"), recid, user)
/*>>>>>PRESUPUESTOVENTA_CONSTRUCTOR*/
        , IIVADesglosable( this, getListPresupuestoVentaDet() )
        , ITotalizableRecord( this, getListPresupuestoVentaDet(), venta )
    {
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "PRESUPUESTO" );
    }
    /*<<<<<PRESUPUESTOVENTA_MEMBERS*/
	void init();
/*>>>>>PRESUPUESTOVENTA_MEMBERS*/
    /*<<<<<PRESUPUESTOVENTA_RELATIONS*/
	RecTipoDoc *getRecTipoDoc() const;
	RecCliente *getRecCliente() const;
	RecAgente *getRecAgente() const;
	RecPresupuestoVentaDet *getRecPresupuestoVentaDet( int npresupuestoventadet = -1 ) const;
	dbRecordList *getListPresupuestoVentaDet() const;
/*>>>>>PRESUPUESTOVENTA_RELATIONS*/
}; // end class

/*<<<<<PRESUPUESTOVENTA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECPRESUPUESTOVENTA_H
/*>>>>>PRESUPUESTOVENTA_POSTAMBLE*/
