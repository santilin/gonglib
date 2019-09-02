/*<<<<<COPYLEFT*/
/** @file ofertasrecofertacompradet.h Registro de detalles ofertas de compra
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
/*<<<<<OFERTACOMPRADET_PREAMBLE*/
#ifndef _OFERTAS_OFERTASRECOFERTACOMPRADET_H
#define _OFERTAS_OFERTASRECOFERTACOMPRADET_H
/*>>>>>OFERTACOMPRADET_PREAMBLE*/

/*<<<<<OFERTACOMPRADET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>OFERTACOMPRADET_INCLUDES*/

namespace gong {
namespace ofertas {

/*<<<<<OFERTACOMPRADET_CONSTRUCTOR*/
class RecOfertaCompraDet: public dbRecord
{
public:
	RecOfertaCompraDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("OFERTACOMPRADET"), recid, user)
/*>>>>>OFERTACOMPRADET_CONSTRUCTOR*/
    {
        addSemanticProperty( "COMPRA" );
        addSemanticProperty( "OFERTA" );
        addSemanticProperty( "DETALLE" );		
	};
    /*<<<<<OFERTACOMPRADET_RELATIONS*/
	factu::RecArticulo *getRecArticulo() const;
	empresa::RecTipoIVA *getRecTipoIVA() const;
/*>>>>>OFERTACOMPRADET_RELATIONS*/
    /*<<<<<OFERTACOMPRADET_MEMBERS*/
	virtual void beforeSaveOrDeleteRelated(dbRecord *master, bool saving); // from dbRecord
/*>>>>>OFERTACOMPRADET_MEMBERS*/
    bool actStocks( dbRecord *ofertacompra, bool savingdetail ) const;
}; // end class

/*<<<<<OFERTACOMPRADET_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASRECOFERTACOMPRADET_H
/*>>>>>OFERTACOMPRADET_POSTAMBLE*/
