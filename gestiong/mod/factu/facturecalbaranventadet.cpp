/*<<<<<COPYLEFT*/
/** @file facturecalbaranventadet.cpp Registro de detalles de albaranes de venta
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
// COPYLEFT Registro de detalles de albaranes de venta
// MEMBER beforeSaveOrDeleteRelated
// RELATION Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::AlbaranVentaDet
/*>>>>>MODULE_INFO*/

/*<<<<<ALBARANVENTADET_INCLUDES*/
#include "facturecalbaranventadet.h"
/*>>>>>ALBARANVENTADET_INCLUDES*/
#include "facturecalbaranventa.h"

namespace gong {
namespace factu {

/*<<<<<ALBARANVENTADET_RELATIONS*/
RecArticulo *RecAlbaranVentaDet::getRecArticulo() const
{
    return static_cast<RecArticulo*>(findRelatedRecord("ALBARANVENTADET.ARTICULO_ID"));
}

empresa::RecTipoIVA *RecAlbaranVentaDet::getRecTipoIVA() const
{
    return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("ALBARANVENTADET.TIPOIVA_ID"));
}

/*>>>>>ALBARANVENTADET_RELATIONS*/

/*<<<<<ALBARANVENTADET_BEFORESAVEORDELETERELATED*/
void RecAlbaranVentaDet::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
    /*>>>>>ALBARANVENTADET_BEFORESAVEORDELETERELATED*/
    actStocks( master, saving );
}

bool RecAlbaranVentaDet::actStocks( dbRecord *albaranventa, bool savingdetail ) const
{
    if( static_cast<RecAlbaranVenta *>(albaranventa)->getRecTipoDoc()->getValue("ACTUALIZASTOCKS").toBool() )
        getRecArticulo()->actStocks(savingdetail, false /*venta*/, getValue( "CANTIDAD" ).toDouble());
    return true;
}

} // namespace factu
} // namespace gong
