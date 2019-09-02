/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de pedidos web
// MEMBER init
// RELATION factu::Agente
// RELATION PedidoWebDet Detalles
// INTERFACE public factu::ITotalizableRecord
// TYPE dbRecord ventasweb::PedidoWeb
/*>>>>>MODULE_INFO*/

/*<<<<<PEDIDOWEB_INCLUDES*/
#include "ventaswebrecpedidoweb.h"
/*>>>>>PEDIDOWEB_INCLUDES*/
#include "empresamodule.h"


namespace gong {
namespace ventasweb {

/*<<<<<PEDIDOWEB_INIT*/
void RecPedidoWeb::init()
{
/*>>>>>PEDIDOWEB_INIT*/
    addStructuralFilter( "PEDIDOWEB.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

/*<<<<<PEDIDOWEB_RELATIONS*/
factu::RecAgente *RecPedidoWeb::getRecAgente() const
{
	return static_cast<factu::RecAgente*>(findRelatedRecord("AGENTE_ID"));
}

RecPedidoWebDet *RecPedidoWeb::getRecPedidoWebDet( int pedidowebdet ) const
{
	return static_cast<RecPedidoWebDet*>(findRelationByRelatedTable("PEDIDOWEBDET" )->getRelatedRecord( pedidowebdet));
}

dbRecordList *RecPedidoWeb::getListPedidoWebDet() const
{
	return findRelationByRelatedTable( "PEDIDOWEBDET" )->getRelatedRecordList();
}
/*>>>>>PEDIDOWEB_RELATIONS*/

} // namespace ventasweb
} // namespace gong
