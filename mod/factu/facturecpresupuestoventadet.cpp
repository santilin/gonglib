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
	return static_cast<RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

empresa::RecTipoIVA *RecPresupuestoVentaDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>PRESUPUESTOVENTADET_RELATIONS*/

} // namespace factu
} // namespace gong

