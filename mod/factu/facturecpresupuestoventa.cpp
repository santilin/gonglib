/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de presupuestos de venta
// MEMBER init
// RELATION TipoDoc
// RELATION Cliente
// RELATION Agente
// RELATION PresupuestoVentaDet Detalles
// INTERFACE public IIVADesglosable
// INTERFACE public ITotalizableRecord
// TYPE dbRecord factu::PresupuestoVenta
/*>>>>>MODULE_INFO*/

/*<<<<<PRESUPUESTOVENTA_INCLUDES*/
#include "facturecpresupuestoventa.h"
/*>>>>>PRESUPUESTOVENTA_INCLUDES*/
#include "empresamodule.h"

namespace gong {
namespace factu {

/*<<<<<PRESUPUESTOVENTA_INIT*/
void RecPresupuestoVenta::init()
{
/*>>>>>PRESUPUESTOVENTA_INIT*/
    addStructuralFilter( "PRESUPUESTOVENTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

/*<<<<<PRESUPUESTOVENTA_RELATIONS*/
RecTipoDoc *RecPresupuestoVenta::getRecTipoDoc() const
{
	return static_cast<RecTipoDoc*>(findRelatedRecord("PRESUPUESTOVENTA.TIPODOC_ID"));
}

RecCliente *RecPresupuestoVenta::getRecCliente() const
{
	return static_cast<RecCliente*>(findRelatedRecord("PRESUPUESTOVENTA.CLIENTE_ID"));
}

RecAgente *RecPresupuestoVenta::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("PRESUPUESTOVENTA.AGENTE_ID"));
}

RecPresupuestoVentaDet *RecPresupuestoVenta::getRecPresupuestoVentaDet( int presupuestoventadet ) const
{
	return static_cast<RecPresupuestoVentaDet*>(findRelationByRelatedTable("PRESUPUESTOVENTADET" )->getRelatedRecord( presupuestoventadet));
}

dbRecordList *RecPresupuestoVenta::getListPresupuestoVentaDet() const
{
	return findRelationByRelatedTable( "PRESUPUESTOVENTADET" )->getRelatedRecordList();
}
/*>>>>>PRESUPUESTOVENTA_RELATIONS*/

} // namespace factu
} // namespace gong

