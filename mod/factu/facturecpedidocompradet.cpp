/*<<<<<COPYLEFT*/
/** @file facturecpedidocompradet.cpp Registro de detalles de pedidos de compra
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
// COPYLEFT Registro de detalles de pedidos de compra
// RELATION Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::PedidoCompraDet
/*>>>>>MODULE_INFO*/

/* El stock y precios del artículo no se actualizan en los pedidos de compra */

/*<<<<<PEDIDOCOMPRADET_INCLUDES*/
#include "facturecpedidocompradet.h"
/*>>>>>PEDIDOCOMPRADET_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<PEDIDOCOMPRADET_RELATIONS*/
RecArticulo *RecPedidoCompraDet::getRecArticulo() const
{
	return static_cast<RecArticulo*>(findRelatedRecord("PEDIDOCOMPRADET.ARTICULO_ID"));
}

empresa::RecTipoIVA *RecPedidoCompraDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("PEDIDOCOMPRADET.TIPOIVA_ID"));
}

/*>>>>>PEDIDOCOMPRADET_RELATIONS*/

} // namespace factu
} // namespace gong
