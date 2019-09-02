/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de detalles de los pedidos web
// RELATION factu::Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord ventasweb::PedidoWebDet
/*>>>>>MODULE_INFO*/

/*<<<<<PEDIDOWEBDET_INCLUDES*/
#include "ventaswebrecpedidowebdet.h"
/*>>>>>PEDIDOWEBDET_INCLUDES*/

namespace gong {
namespace ventasweb {
/*<<<<<PEDIDOWEBDET_RELATIONS*/
factu::RecArticulo *RecPedidoWebDet::getRecArticulo() const
{
	return static_cast<factu::RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

empresa::RecTipoIVA *RecPedidoWebDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>PEDIDOWEBDET_RELATIONS*/
} // namespace ventasweb
} // namespace gong
