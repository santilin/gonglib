/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de pedidos de compra
// MEMBER init
// RELATION TipoDoc
// RELATION Proveedora
// RELATION Agente
// RELATION PedidoCompraDet Detalles
// INTERFACE public ITotalizableRecord
// TYPE dbRecord factu::PedidoCompra
/*>>>>>MODULE_INFO*/

/*<<<<<PEDIDOCOMPRA_INCLUDES*/
#include "facturecpedidocompra.h"
/*>>>>>PEDIDOCOMPRA_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace factu {

/*<<<<<PEDIDOCOMPRA_INIT*/
void RecPedidoCompra::init()
{
/*>>>>>PEDIDOCOMPRA_INIT*/
    addStructuralFilter( "PEDIDOCOMPRA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "PEDIDOCOMPRA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

/*<<<<<PEDIDOCOMPRA_RELATIONS*/
RecTipoDoc *RecPedidoCompra::getRecTipoDoc() const
{
	return static_cast<RecTipoDoc*>(findRelatedRecord("TIPODOC_ID"));
}

RecProveedora *RecPedidoCompra::getRecProveedora() const
{
	return static_cast<RecProveedora*>(findRelatedRecord("PROVEEDORA_ID"));
}

RecAgente *RecPedidoCompra::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("AGENTE_ID"));
}

RecPedidoCompraDet *RecPedidoCompra::getRecPedidoCompraDet( int pedidocompradet ) const
{
	return static_cast<RecPedidoCompraDet*>(findRelationByRelatedTable("PEDIDOCOMPRADET" )->getRelatedRecord( pedidocompradet));
}

dbRecordList *RecPedidoCompra::getListPedidoCompraDet() const
{
	return findRelationByRelatedTable( "PEDIDOCOMPRADET" )->getRelatedRecordList();
}
/*>>>>>PEDIDOCOMPRA_RELATIONS*/

} // namespace factu
} // namespace gong
