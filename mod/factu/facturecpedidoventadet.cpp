/*<<<<<COPYLEFT*/
/** @file facturecpedidoventadet.cpp Registro de detalles de pedidos de venta
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
// COPYLEFT Registro de detalles de pedidos de venta
// RELATION Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::PedidoVentaDet
/*>>>>>MODULE_INFO*/

/* El stock y los precios del artículo no se actualizan en los pedidos de venta */

/*<<<<<PEDIDOVENTADET_INCLUDES*/
#include "facturecpedidoventadet.h"
/*>>>>>PEDIDOVENTADET_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<PEDIDOVENTADET_RELATIONS*/
RecArticulo *RecPedidoVentaDet::getRecArticulo() const
{
	return static_cast<RecArticulo*>(findRelatedRecord("PEDIDOVENTADET.ARTICULO_ID"));
}

empresa::RecTipoIVA *RecPedidoVentaDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("PEDIDOVENTADET.TIPOIVA_ID"));
}

/*>>>>>PEDIDOVENTADET_RELATIONS*/

} // namespace factu
} // namespace gong
