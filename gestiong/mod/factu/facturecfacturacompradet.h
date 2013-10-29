/*<<<<<COPYLEFT*/
/** @file facturecfacturacompradet.h Registro de detalles de facturas de compra
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<FACTURACOMPRADET_PREAMBLE*/
#ifndef _FACTU_FACTURECFACTURACOMPRADET_H
#define _FACTU_FACTURECFACTURACOMPRADET_H
/*>>>>>FACTURACOMPRADET_PREAMBLE*/

/*<<<<<FACTURACOMPRADET_INCLUDES*/
#include <dbappdbapplication.h>

#include "facturecarticulo.h"
#include "empresarectipoiva.h"
/*>>>>>FACTURACOMPRADET_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<FACTURACOMPRADET_CONSTRUCTOR*/
class RecFacturaCompraDet: public dbRecord
{
public:
    RecFacturaCompraDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("FACTURACOMPRADET"), recid, user)
/*>>>>>FACTURACOMPRADET_CONSTRUCTOR*/
    {
        addSemanticProperty( "COMPRA" );
        addSemanticProperty( "FACTURA" );
        addSemanticProperty( "DETALLE" );
    }

    /*<<<<<FACTURACOMPRADET_MEMBERS*/
    virtual void beforeSaveOrDeleteRelated(dbRecord *master, bool saving); // from dbRecord
    /*>>>>>FACTURACOMPRADET_MEMBERS*/

    bool actStocks(dbRecord *facturacompra, bool savingdetail) const;
    bool actAlbaranes( dbRecord *facturacompra, bool savingdetail ) const;

    /*<<<<<FACTURACOMPRADET_RELATIONS*/
    RecArticulo *getRecArticulo() const;
    empresa::RecTipoIVA *getRecTipoIVA() const;
    /*>>>>>FACTURACOMPRADET_RELATIONS*/

}; // end class

/*<<<<<FACTURACOMPRADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECFACTURACOMPRADET_H
/*>>>>>FACTURACOMPRADET_POSTAMBLE*/

