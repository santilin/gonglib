/*<<<<<COPYLEFT*/
/** @file facturecalbarancompradet.h Registro de detalles de albaranes de compra
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
/*<<<<<ALBARANCOMPRADET_PREAMBLE*/
#ifndef _FACTU_FACTURECALBARANCOMPRADET_H
#define _FACTU_FACTURECALBARANCOMPRADET_H
/*>>>>>ALBARANCOMPRADET_PREAMBLE*/

/*<<<<<ALBARANCOMPRADET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>ALBARANCOMPRADET_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<ALBARANCOMPRADET_CONSTRUCTOR*/
class RecAlbaranCompraDet: public dbRecord
{
public:
	RecAlbaranCompraDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("ALBARANCOMPRADET"), recid, user)
/*>>>>>ALBARANCOMPRADET_CONSTRUCTOR*/
    {
        addSemanticProperty( "COMPRA" );
        addSemanticProperty( "ALBARAN" );
        addSemanticProperty( "DETALLE" );
    }
    /*<<<<<ALBARANCOMPRADET_RELATIONS*/
	RecArticulo *getRecArticulo() const;
	empresa::RecTipoIVA *getRecTipoIVA() const;
/*>>>>>ALBARANCOMPRADET_RELATIONS*/

    /*<<<<<ALBARANCOMPRADET_MEMBERS*/
	virtual void beforeSaveOrDeleteRelated(dbRecord *master, bool saving); // from dbRecord
/*>>>>>ALBARANCOMPRADET_MEMBERS*/
    bool actStocks( dbRecord *albarancompra, bool savingdetail ) const;
}; // end class

/*<<<<<ALBARANCOMPRADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECALBARANCOMPRADET_H
/*>>>>>ALBARANCOMPRADET_POSTAMBLE*/
