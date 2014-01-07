/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de pedidos de venta
// MEMBER init
// RELATION TipoDoc
// RELATION Cliente
// RELATION Agente
// RELATION PedidoVentaDet Detalles
// INTERFACE public IIVADesglosable
// INTERFACE public ITotalizableRecord
// TYPE dbRecord factu::PedidoVenta
/*>>>>>MODULE_INFO*/

/*<<<<<PEDIDOVENTA_INCLUDES*/
#include "facturecpedidoventa.h"
/*>>>>>PEDIDOVENTA_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace factu {

/*<<<<<PEDIDOVENTA_INIT*/
void RecPedidoVenta::init()
{
/*>>>>>PEDIDOVENTA_INIT*/
    addStructuralFilter( "PEDIDOVENTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "PEDIDOVENTA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}


/*<<<<<PEDIDOVENTA_RELATIONS*/
RecTipoDoc *RecPedidoVenta::getRecTipoDoc() const
{
	return static_cast<RecTipoDoc*>(findRelatedRecord("PEDIDOVENTA.TIPODOC_ID"));
}

RecCliente *RecPedidoVenta::getRecCliente() const
{
	return static_cast<RecCliente*>(findRelatedRecord("PEDIDOVENTA.CLIENTE_ID"));
}

RecAgente *RecPedidoVenta::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("PEDIDOVENTA.AGENTE_ID"));
}

RecPedidoVentaDet *RecPedidoVenta::getRecPedidoVentaDet( int pedidoventadet ) const
{
	return static_cast<RecPedidoVentaDet*>(findRelationByRelatedTable("PEDIDOVENTADET" )->getRelatedRecord( pedidoventadet));
}

dbRecordList *RecPedidoVenta::getListPedidoVentaDet() const
{
	return findRelationByRelatedTable( "PEDIDOVENTADET" )->getRelatedRecordList();
}
/*>>>>>PEDIDOVENTA_RELATIONS*/

} // namespace factu
} // namespace gong
