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
	return static_cast<RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

empresa::RecTipoIVA *RecPedidoCompraDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>PEDIDOCOMPRADET_RELATIONS*/

} // namespace factu
} // namespace gong
