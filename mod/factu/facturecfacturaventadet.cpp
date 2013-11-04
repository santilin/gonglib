/*<<<<<COPYLEFT*/
/** @file facturecfacturaventadet.cpp Registro de detalles de facturas de venta
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
// COPYLEFT Registro de detalles de facturas de venta
// MEMBER beforeSaveOrDeleteRelated
// RELATION Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::FacturaVentaDet
/*>>>>>MODULE_INFO*/

/*<<<<<FACTURAVENTADET_INCLUDES*/
#include "facturecfacturaventadet.h"
/*>>>>>FACTURAVENTADET_INCLUDES*/
#include "facturecfacturaventa.h"

namespace gong {
namespace factu {

/*<<<<<FACTURAVENTADET_BEFORESAVEORDELETERELATED*/
void RecFacturaVentaDet::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
/*>>>>>FACTURAVENTADET_BEFORESAVEORDELETERELATED*/
    actStocks( master, saving );
    actAlbaranes( master, saving );
}

bool RecFacturaVentaDet::actAlbaranes(dbRecord *facturaventa, bool saving) const
{
    dbRecordID alb_id = getValue( "ALBARANVENTA_ID" ).toInt();
    if( alb_id != 0 ) {
        Xtring sql = "UPDATE ALBARANVENTA SET FACTURADO=" + getConnection()->toSQL( saving )
                     + " WHERE ID=" + getConnection()->toSQL( alb_id );
        getConnection()->exec( sql );
    }
    return true;
}

bool RecFacturaVentaDet::actStocks( dbRecord *facturaventa, bool saving ) const
{
    double cantidad;
    if( static_cast<RecFacturaVenta *>(facturaventa)->getRecTipoDoc()->getValue("ACTUALIZASTOCKS").toBool() )
        cantidad = getValue( "CANTIDAD" ).toDouble();
    else
        cantidad = 0.0;
    if( getValue( "ALBARANVENTA_ID" ).toInt() == 0 ) {
        // Las líneas que proceden de albaranes no cambian el stock, ya lo hicieron en el albarán
        getRecArticulo()->actStocks(saving, false /*venta*/, cantidad);
    }
    return true;
}

/*<<<<<FACTURAVENTADET_RELATIONS*/
RecArticulo *RecFacturaVentaDet::getRecArticulo() const
{
	return static_cast<RecArticulo*>(findRelatedRecord("FACTURAVENTADET.ARTICULO_ID"));
}

empresa::RecTipoIVA *RecFacturaVentaDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("FACTURAVENTADET.TIPOIVA_ID"));
}

/*>>>>>FACTURAVENTADET_RELATIONS*/

} // namespace factu
} // namespace gong
