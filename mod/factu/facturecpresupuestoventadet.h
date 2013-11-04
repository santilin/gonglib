/*<<<<<COPYLEFT*/
/** @file facturecpresupuestoventadet.h Registro de detalles de presupuestos de venta
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
/*<<<<<PRESUPUESTOVENTADET_PREAMBLE*/
#ifndef _FACTU_FACTURECPRESUPUESTOVENTADET_H
#define _FACTU_FACTURECPRESUPUESTOVENTADET_H
/*>>>>>PRESUPUESTOVENTADET_PREAMBLE*/

/*<<<<<PRESUPUESTOVENTADET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>PRESUPUESTOVENTADET_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<PRESUPUESTOVENTADET_CONSTRUCTOR*/
class RecPresupuestoVentaDet: public dbRecord
{
public:
	RecPresupuestoVentaDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PRESUPUESTOVENTADET"), recid, user)
/*>>>>>PRESUPUESTOVENTADET_CONSTRUCTOR*/
    {
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "PRESUPUESTO" );
        addSemanticProperty( "DETALLE" );
    }
    /*<<<<<PRESUPUESTOVENTADET_RELATIONS*/
	RecArticulo *getRecArticulo() const;
	empresa::RecTipoIVA *getRecTipoIVA() const;
/*>>>>>PRESUPUESTOVENTADET_RELATIONS*/
}; // end class

/*<<<<<PRESUPUESTOVENTADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECPRESUPUESTOVENTADET_H
/*>>>>>PRESUPUESTOVENTADET_POSTAMBLE*/

