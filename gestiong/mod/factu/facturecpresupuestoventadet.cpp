/*<<<<<COPYLEFT*/
/** @file facturecpresupuestoventadet.cpp Registro de detalles de presupuestos de venta
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
// COPYLEFT Registro de detalles de presupuestos de venta
// RELATION Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::PresupuestoVentaDet
/*>>>>>MODULE_INFO*/

/* En los presupuestos de venta no se actualiza nada de los artículos */

/*<<<<<PRESUPUESTOVENTADET_INCLUDES*/
#include "facturecpresupuestoventadet.h"
/*>>>>>PRESUPUESTOVENTADET_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<PRESUPUESTOVENTADET_RELATIONS*/
RecArticulo *RecPresupuestoVentaDet::getRecArticulo() const
{
    return static_cast<RecArticulo*>(findRelatedRecord("PRESUPUESTOVENTADET.ARTICULO_ID"));
}

empresa::RecTipoIVA *RecPresupuestoVentaDet::getRecTipoIVA() const
{
    return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("PRESUPUESTOVENTADET.TIPOIVA_ID"));
}

/*>>>>>PRESUPUESTOVENTADET_RELATIONS*/

} // namespace factu
} // namespace gong

