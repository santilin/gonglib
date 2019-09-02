/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de pedidos de compra
// MEMBER init
// MEMBER remove
// RELATION factu::TipoDoc
// RELATION factu::Proveedora
// RELATION LiquidacionCompraDet Detalles
// INTERFACE public factu::ITotalizableRecord
// TYPE dbRecord deposit::LiquidacionCompra
/*>>>>>MODULE_INFO*/

/*<<<<<LIQUIDACIONCOMPRA_INCLUDES*/
#include "depositrecliquidacioncompra.h"
/*>>>>>LIQUIDACIONCOMPRA_INCLUDES*/
#include <empresamodule.h>
#include <factumodule.h>

namespace gong {
namespace deposit {

/*<<<<<LIQUIDACIONCOMPRA_INIT*/
void RecLiquidacionCompra::init()
{
/*>>>>>LIQUIDACIONCOMPRA_INIT*/
    addStructuralFilter( "LIQUIDACIONCOMPRA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
	addStructuralFilter( "LIQUIDACIONCOMPRA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

/*<<<<<LIQUIDACIONCOMPRA_RELATIONS*/
factu::RecTipoDoc *RecLiquidacionCompra::getRecTipoDoc() const
{
	return static_cast<factu::RecTipoDoc*>(findRelatedRecord("TIPODOC_ID"));
}

factu::RecProveedora *RecLiquidacionCompra::getRecProveedora() const
{
	return static_cast<factu::RecProveedora*>(findRelatedRecord("PROVEEDORA_ID"));
}

RecLiquidacionCompraDet *RecLiquidacionCompra::getRecLiquidacionCompraDet( int liquidacioncompradet ) const
{
	return static_cast<RecLiquidacionCompraDet*>(findRelationByRelatedTable("LIQUIDACIONCOMPRADET" )->getRelatedRecord( liquidacioncompradet));
}

dbRecordList *RecLiquidacionCompra::getListLiquidacionCompraDet() const
{
	return findRelationByRelatedTable( "LIQUIDACIONCOMPRADET" )->getRelatedRecordList();
}
/*>>>>>LIQUIDACIONCOMPRA_RELATIONS*/


/**
 * @brief Borra el campo LIQUIDACIONCOMPRA_ID de ALBARANVENTADET y FACTURAVENTADET
 *
 * @return bool
 **/
/*<<<<<LIQUIDACIONCOMPRA_REMOVE*/
bool RecLiquidacionCompra::remove() throw( dbError )
{
/*>>>>>LIQUIDACIONCOMPRA_REMOVE*/
	dbRecordID id = getRecordID();
	bool ret = dbRecord::remove();
	if( ret ) {
		getConnection()->exec( "UPDATE ALBARANVENTADET SET LIQUIDACIONCOMPRA_ID = NULL"
					" WHERE LIQUIDACIONCOMPRA_ID=" + getConnection()->toSQL( id ) );
		long int nr = getConnection()->getAffectedRows();
		getConnection()->exec( "UPDATE FACTURAVENTADET SET LIQUIDACIONCOMPRA_ID = NULL"
					" WHERE LIQUIDACIONCOMPRA_ID=" + getConnection()->toSQL( id ) );
		nr += getConnection()->getAffectedRows();
		DBAPP->showStickyOSD( Xtring::null, Xtring::printf( _("%ld %s anuladas en %s y %s"), nr,
											DBAPP->getTableDescPlural( "LIQUIDACIONCOMPRA", "" ).c_str(),
											DBAPP->getTableDescPlural( "ALBARANVENTA", "" ).c_str(),
											DBAPP->getTableDescPlural( "FACTURAVENTA", "" ).c_str() ) );
	}
	return ret;
}


} // namespace deposit
} // namespace gong
