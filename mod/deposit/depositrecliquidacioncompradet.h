/*<<<<<COPYLEFT*/
/** @file depositrecliquidacioncompradet.h Registro de detalles de pedidos de compra
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
/*<<<<<LIQUIDACIONCOMPRADET_PREAMBLE*/
#ifndef _DEPOSIT_DEPOSITRECLIQUIDACIONCOMPRADET_H
#define _DEPOSIT_DEPOSITRECLIQUIDACIONCOMPRADET_H
/*>>>>>LIQUIDACIONCOMPRADET_PREAMBLE*/

/*<<<<<LIQUIDACIONCOMPRADET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>LIQUIDACIONCOMPRADET_INCLUDES*/

namespace gong {
namespace deposit {

/*<<<<<LIQUIDACIONCOMPRADET_CONSTRUCTOR*/
class RecLiquidacionCompraDet: public dbRecord
{
public:
	RecLiquidacionCompraDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("LIQUIDACIONCOMPRADET"), recid, user)
/*>>>>>LIQUIDACIONCOMPRADET_CONSTRUCTOR*/
    {
		addSemanticProperty( "COMPRA" );
		addSemanticProperty( "LIQUIDACION" );
		addSemanticProperty( "DETALLE" );
	};
/*<<<<<LIQUIDACIONCOMPRADET_RELATIONS*/
	factu::RecArticulo *getRecArticulo() const;
	empresa::RecTipoIVA *getRecTipoIVA() const;
/*>>>>>LIQUIDACIONCOMPRADET_RELATIONS*/
}; // end class

/*<<<<<LIQUIDACIONCOMPRADET_POSTAMBLE*/
} // namespace deposit
} // namespace gong
#endif // DEPOSIT_DEPOSITRECLIQUIDACIONCOMPRADET_H
/*>>>>>LIQUIDACIONCOMPRADET_POSTAMBLE*/
