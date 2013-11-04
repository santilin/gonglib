/*<<<<<COPYLEFT*/
/** @file facturecfacturacompradet.cpp Registro de detalles de facturas de compra
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de detalles de facturas de compra
// MEMBER beforeSaveOrDeleteRelated
// RELATION Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::FacturaCompraDet
/*>>>>>MODULE_INFO*/

/*<<<<<FACTURACOMPRADET_INCLUDES*/
#include "facturecfacturacompradet.h"
/*>>>>>FACTURACOMPRADET_INCLUDES*/
#include "facturecfacturacompra.h"

namespace gong {
namespace factu {

bool RecFacturaCompraDet::actAlbaranes(dbRecord *facturacompra, bool saving) const
{
    dbRecordID alb_id = getValue( "ALBARANCOMPRA_ID" ).toInt();
    if( alb_id != 0 ) {
        Xtring sql = "UPDATE ALBARANCOMPRA SET FACTURADO=" + getConnection()->toSQL( saving )
                     + " WHERE ID=" + getConnection()->toSQL( alb_id );
        getConnection()->exec( sql );
    }
    return true;
}

bool RecFacturaCompraDet::actStocks( dbRecord *facturacompra, bool saving ) const
{
    double cantidad;
    if( static_cast<RecFacturaCompra *>(facturacompra)->getRecTipoDoc()->getValue("ACTUALIZASTOCKS").toBool() )
        cantidad = getValue( "CANTIDAD" ).toDouble();
    else
        cantidad = 0.0;
    getRecArticulo()->actStocks( saving, true /*compra*/, cantidad);
    return true;
}


/*<<<<<FACTURACOMPRADET_RELATIONS*/
RecArticulo *RecFacturaCompraDet::getRecArticulo() const
{
	return static_cast<RecArticulo*>(findRelatedRecord("FACTURACOMPRADET.ARTICULO_ID"));
}

empresa::RecTipoIVA *RecFacturaCompraDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("FACTURACOMPRADET.TIPOIVA_ID"));
}

/*>>>>>FACTURACOMPRADET_RELATIONS*/

/*<<<<<FACTURACOMPRADET_BEFORESAVEORDELETERELATED*/
void RecFacturaCompraDet::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
/*>>>>>FACTURACOMPRADET_BEFORESAVEORDELETERELATED*/
    actStocks( master, saving );
    actAlbaranes( master, saving );
}

} // namespace factu
} // namespace gong
