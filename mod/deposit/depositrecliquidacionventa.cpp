/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de pedidos de venta
// MEMBER init
// MEMBER save
// RELATION factu::TipoDoc
// RELATION factu::Cliente
// RELATION factu::Agente
// RELATION LiquidacionVentaDet Detalles
// INTERFACE public factu::ITotalizableRecord
// TYPE dbRecord deposit::LiquidacionVenta
/*>>>>>MODULE_INFO*/

/*<<<<<LIQUIDACIONVENTA_INCLUDES*/
#include "depositrecliquidacionventa.h"
/*>>>>>LIQUIDACIONVENTA_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace deposit {

/*<<<<<LIQUIDACIONVENTA_INIT*/
void RecLiquidacionVenta::init()
{
/*>>>>>LIQUIDACIONVENTA_INIT*/
    addStructuralFilter( "LIQUIDACIONVENTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
	addStructuralFilter( "LIQUIDACIONVENTA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

/*<<<<<LIQUIDACIONVENTA_RELATIONS*/
factu::RecTipoDoc *RecLiquidacionVenta::getRecTipoDoc() const
{
	return static_cast<factu::RecTipoDoc*>(findRelatedRecord("TIPODOC_ID"));
}

factu::RecCliente *RecLiquidacionVenta::getRecCliente() const
{
	return static_cast<factu::RecCliente*>(findRelatedRecord("CLIENTE_ID"));
}

factu::RecAgente *RecLiquidacionVenta::getRecAgente() const
{
	return static_cast<factu::RecAgente*>(findRelatedRecord("AGENTE_ID"));
}

RecLiquidacionVentaDet *RecLiquidacionVenta::getRecLiquidacionVentaDet( int liquidacionventadet ) const
{
	return static_cast<RecLiquidacionVentaDet*>(findRelationByRelatedTable("LIQUIDACIONVENTADET" )->getRelatedRecord( liquidacionventadet));
}

dbRecordList *RecLiquidacionVenta::getListLiquidacionVentaDet() const
{
	return findRelationByRelatedTable( "LIQUIDACIONVENTADET" )->getRelatedRecordList();
}
/*>>>>>LIQUIDACIONVENTA_RELATIONS*/

/*<<<<<LIQUIDACIONVENTA_SAVE*/
bool RecLiquidacionVenta::save(bool validate, bool saverelated = true) throw( dbError )
{
/*>>>>>LIQUIDACIONVENTA_SAVE*/
	if( getValue( "CONTADOR" ).toInt() == 0 )
		setValue( "CONTADOR", empresa::ModuleInstance->getMaxContador() );
	bool ret = dbRecord::save(saverelated);
	return ret;
}

} // namespace deposit
} // namespace gong

