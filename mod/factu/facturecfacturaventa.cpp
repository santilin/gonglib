/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de facturas de venta
// MEMBER init
// MEMBER save
// MEMBER remove
// MEMBER toString
// RELATION TipoDoc
// RELATION Cliente
// RELATION Agente
// RELATION pagos::FormaPago
// RELATION empresa::Proyecto
// RELATION FacturaVentaDet Detalles
// INTERFACE public factu::IPagableFactura
// INTERFACE public factu::IIVADesglosable
// INTERFACE public factu::IAsentableFactura #ifdef|HAVE_CONTABMODULE
// INTERFACE public tesoreria::IApuntableRecord #ifdef|HAVE_TESORERIAMODULE
// INTERFACE public factu::ITotalizableRecord
// TYPE dbRecord factu::FacturaVenta
/*>>>>>MODULE_INFO*/

/*<<<<<FACTURAVENTA_INCLUDES*/
#include "facturecfacturaventa.h"
/*>>>>>FACTURAVENTA_INCLUDES*/
#include <empresamodule.h>

#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#include <contabrecasiento.h>
#endif
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<FACTURAVENTA_INIT*/
void RecFacturaVenta::init()
{
/*>>>>>FACTURAVENTA_INIT*/
    addStructuralFilter( "FACTURAVENTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "FACTURAVENTA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}


/*<<<<<FACTURAVENTA_RELATIONS*/
RecTipoDoc *RecFacturaVenta::getRecTipoDoc() const
{
	return static_cast<RecTipoDoc*>(findRelatedRecord("TIPODOC_ID"));
}

RecCliente *RecFacturaVenta::getRecCliente() const
{
	return static_cast<RecCliente*>(findRelatedRecord("CLIENTE_ID"));
}

RecAgente *RecFacturaVenta::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("AGENTE_ID"));
}

pagos::RecFormaPago *RecFacturaVenta::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("FORMAPAGO_ID"));
}

empresa::RecProyecto *RecFacturaVenta::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

RecFacturaVentaDet *RecFacturaVenta::getRecFacturaVentaDet( int facturaventadet ) const
{
	return static_cast<RecFacturaVentaDet*>(findRelationByRelatedTable("FACTURAVENTADET" )->getRelatedRecord( facturaventadet));
}

dbRecordList *RecFacturaVenta::getListFacturaVentaDet() const
{
	return findRelationByRelatedTable( "FACTURAVENTADET" )->getRelatedRecordList();
}
/*>>>>>FACTURAVENTA_RELATIONS*/

/*<<<<<FACTURAVENTA_TOSTRING*/
Xtring RecFacturaVenta::toString(int format, const Xtring &includedFields) const
{
	Xtring result;
/*>>>>>FACTURAVENTA_TOSTRING*/
    if( format == TOSTRING_CODE_AND_DESC_WITH_TABLENAME || format == TOSTRING_CODE_AND_DESC || format == TOSTRING_DESC ) {
        result = dbRecord::toString( format, includedFields );
        if( !getRecCliente()->getValue("RAZONSOCIAL").toString().isEmpty() )
            result += ", " + getRecCliente()->getValue("RAZONSOCIAL").toString();
    } else
        result = dbRecord::toString(format, includedFields);
    return result;
}

/*<<<<<FACTURAVENTA_SAVE*/
bool RecFacturaVenta::save(bool saverelated) throw( dbError )
{
/*>>>>>FACTURAVENTA_SAVE*/
	if (saverelated) {
		if( DBAPP->findModule("pagos") ) 
#ifdef HAVE_PAGOSMODULE
			actRestoFactura();
#endif
	}
    bool ret = dbRecord::save(saverelated);
    if( ret && saverelated ) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) {
			delCobros( false );
			genCobros();
		}
#endif
#ifdef HAVE_CONTABMODULE
		if( DBAPP->findModule("contab") )  {
			if( contab::ModuleInstance->isContabActive() ) {
				bool supervisar = contab::ModuleInstance->getModuleSetting( "SUPERVISAR_ASIENTOS" ).toBool();
				regenAsiento( supervisar );
			}
		}
#endif
#ifdef HAVE_TESORERIAMODULE
        if( DBAPP->findModule("tesoreria") ) {
            bool supervisar = tesoreria::ModuleInstance->getModuleSetting( "SUPERVISAR_APUNTES" ).toBool();
            regenApunte( supervisar );
        }
#endif
    }
    return ret;
}

/*<<<<<FACTURAVENTA_REMOVE*/
bool RecFacturaVenta::remove() throw( dbError )
{
/*>>>>>FACTURAVENTA_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) 
			delCobros( true );
#endif
#ifdef HAVE_CONTABMODULE
		if( DBAPP->findModule("contab") ) {
			if( contab::ModuleInstance->isContabActive() )
				delete borraAsiento();
		}
#endif
#ifdef HAVE_TESORERIAMODULE
        if( DBAPP->findModule("tesoreria") ) {
            delete borraApunte( false /*no regenerando*/);
        }
#endif
    }
    return ret;
}

} // namespace factu
} // namespace gong

