/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de detalles de pedidos de compra
// RELATION factu::Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord deposit::LiquidacionCompraDet
/*>>>>>MODULE_INFO*/

/*<<<<<LIQUIDACIONCOMPRADET_INCLUDES*/
#include "depositrecliquidacioncompradet.h"
/*>>>>>LIQUIDACIONCOMPRADET_INCLUDES*/

namespace gong {
namespace deposit {

/*<<<<<LIQUIDACIONCOMPRADET_RELATIONS*/
factu::RecArticulo *RecLiquidacionCompraDet::getRecArticulo() const
{
	return static_cast<factu::RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

empresa::RecTipoIVA *RecLiquidacionCompraDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>LIQUIDACIONCOMPRADET_RELATIONS*/

} // namespace deposit
} // namespace gong
