/*<<<<<COPYLEFT*/
/** @file depositrecliquidacionventadet.h Registro de detalles de pedidos de venta
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
/*<<<<<LIQUIDACIONVENTADET_PREAMBLE*/
#ifndef _DEPOSIT_DEPOSITRECLIQUIDACIONVENTADET_H
#define _DEPOSIT_DEPOSITRECLIQUIDACIONVENTADET_H
/*>>>>>LIQUIDACIONVENTADET_PREAMBLE*/

/*<<<<<LIQUIDACIONVENTADET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>LIQUIDACIONVENTADET_INCLUDES*/

namespace gong {
namespace deposit {

/*<<<<<LIQUIDACIONVENTADET_CONSTRUCTOR*/
class RecLiquidacionVentaDet: public dbRecord
{
public:
	RecLiquidacionVentaDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("LIQUIDACIONVENTADET"), recid, user)
/*>>>>>LIQUIDACIONVENTADET_CONSTRUCTOR*/
	{
		addSemanticProperty( "VENTA" );
		addSemanticProperty( "LIQUIDACION" );
		addSemanticProperty( "DETALLE" );
	}
/*<<<<<LIQUIDACIONVENTADET_RELATIONS*/
	factu::RecArticulo *getRecArticulo() const;
	empresa::RecTipoIVA *getRecTipoIVA() const;
/*>>>>>LIQUIDACIONVENTADET_RELATIONS*/
}; // end class

/*<<<<<LIQUIDACIONVENTADET_POSTAMBLE*/
} // namespace deposit
} // namespace gong
#endif // DEPOSIT_DEPOSITRECLIQUIDACIONVENTADET_H
/*>>>>>LIQUIDACIONVENTADET_POSTAMBLE*/
