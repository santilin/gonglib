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
