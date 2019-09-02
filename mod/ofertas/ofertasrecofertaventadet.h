/*<<<<<COPYLEFT*/
/** @file ofertasrecofertaventadet.h Registro de detalles de ofertas de venta
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
/*<<<<<OFERTAVENTADET_PREAMBLE*/
#ifndef _OFERTAS_OFERTASRECOFERTAVENTADET_H
#define _OFERTAS_OFERTASRECOFERTAVENTADET_H
/*>>>>>OFERTAVENTADET_PREAMBLE*/

/*<<<<<OFERTAVENTADET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>OFERTAVENTADET_INCLUDES*/

namespace gong {
namespace ofertas {

/*<<<<<OFERTAVENTADET_CONSTRUCTOR*/
class RecOfertaVentaDet: public dbRecord
{
public:
	RecOfertaVentaDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("OFERTAVENTADET"), recid, user)
/*>>>>>OFERTAVENTADET_CONSTRUCTOR*/
    {
        addSemanticProperty( "VENTA" );
        addSemanticProperty( "OFERTA" );
        addSemanticProperty( "DETALLE" );
	};
    /*<<<<<OFERTAVENTADET_RELATIONS*/
	factu::RecArticulo *getRecArticulo() const;
	empresa::RecTipoIVA *getRecTipoIVA() const;
/*>>>>>OFERTAVENTADET_RELATIONS*/
    /*<<<<<OFERTAVENTADET_MEMBERS*/
	virtual void beforeSaveOrDeleteRelated(dbRecord *master, bool saving); // from dbRecord
/*>>>>>OFERTAVENTADET_MEMBERS*/
    bool actStocks( dbRecord *ofertaventa, bool savingdetail ) const;
}; // end class

/*<<<<<OFERTAVENTADET_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASRECOFERTAVENTADET_H
/*>>>>>OFERTAVENTADET_POSTAMBLE*/
