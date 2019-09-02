/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de detalles de pedidos de venta
// RELATION factu::Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord deposit::LiquidacionVentaDet
/*>>>>>MODULE_INFO*/

/*<<<<<LIQUIDACIONVENTADET_INCLUDES*/
#include "depositrecliquidacionventadet.h"
/*>>>>>LIQUIDACIONVENTADET_INCLUDES*/

namespace gong {
namespace deposit {

/*<<<<<LIQUIDACIONVENTADET_RELATIONS*/
factu::RecArticulo *RecLiquidacionVentaDet::getRecArticulo() const
{
	return static_cast<factu::RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

empresa::RecTipoIVA *RecLiquidacionVentaDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>LIQUIDACIONVENTADET_RELATIONS*/

} // namespace deposit
} // namespace gong
