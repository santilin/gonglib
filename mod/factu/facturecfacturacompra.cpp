/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de facturas de compra
// MEMBER init
// MEMBER save
// MEMBER remove
// MEMBER toString
// RELATION TipoDoc
// RELATION Proveedora
// RELATION Agente
// RELATION pagos::FormaPago
// RELATION empresa::Proyecto
// RELATION FacturaCompraDet Detalles
// RELATION tesoreria::CuentaTesoreria #ifdef|HAVE_TESORERIAMODULE
// INTERFACE public factu::IPagableFactura
// INTERFACE public factu::IIVADesglosable
// INTERFACE public factu::IAsentableFactura #ifdef|HAVE_CONTABMODULE
// INTERFACE public tesoreria::IApuntableRecord #ifdef|HAVE_TESORERIAMODULE
// INTERFACE public factu::ITotalizableRecord
// TYPE dbRecord factu::FacturaCompra
/*>>>>>MODULE_INFO*/

/*<<<<<FACTURACOMPRA_INCLUDES*/
#include "facturecfacturacompra.h"
/*>>>>>FACTURACOMPRA_INCLUDES*/
#include <empresamodule.h>

#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#include <contabrecasiento.h>
#endif
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<FACTURACOMPRA_INIT*/
void RecFacturaCompra::init()
{
/*>>>>>FACTURACOMPRA_INIT*/
    addStructuralFilter( "FACTURACOMPRA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "FACTURACOMPRA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

/*<<<<<FACTURACOMPRA_RELATIONS$s/CUENTATESORERIA/CUENTAPAGO/$*/
RecTipoDoc *RecFacturaCompra::getRecTipoDoc() const
{
	return static_cast<RecTipoDoc*>(findRelatedRecord("TIPODOC_ID"));
}

RecProveedora *RecFacturaCompra::getRecProveedora() const
{
	return static_cast<RecProveedora*>(findRelatedRecord("PROVEEDORA_ID"));
}

RecAgente *RecFacturaCompra::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("AGENTE_ID"));
}

pagos::RecFormaPago *RecFacturaCompra::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("FORMAPAGO_ID"));
}

empresa::RecProyecto *RecFacturaCompra::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

RecFacturaCompraDet *RecFacturaCompra::getRecFacturaCompraDet( int facturacompradet ) const
{
	return static_cast<RecFacturaCompraDet*>(findRelationByRelatedTable("FACTURACOMPRADET" )->getRelatedRecord( facturacompradet));
}

dbRecordList *RecFacturaCompra::getListFacturaCompraDet() const
{
	return findRelationByRelatedTable( "FACTURACOMPRADET" )->getRelatedRecordList();
}
#ifdef HAVE_TESORERIAMODULE
tesoreria::RecCuentaTesoreria *RecFacturaCompra::getRecCuentaTesoreria() const
{
	return static_cast<tesoreria::RecCuentaTesoreria*>(findRelatedRecord("CUENTAPAGO_ID"));
}

#endif
/*>>>>>FACTURACOMPRA_RELATIONS*/

/*<<<<<FACTURACOMPRA_TOSTRING*/
Xtring RecFacturaCompra::toString(int format, const Xtring &includedFields) const
{
	Xtring result;
/*>>>>>FACTURACOMPRA_TOSTRING*/
    if( format == TOSTRING_CODE_AND_DESC_WITH_TABLENAME || format == TOSTRING_CODE_AND_DESC || format == TOSTRING_DESC ) {
        result = dbRecord::toString( format, includedFields );
        if( !getRecProveedora()->getValue("RAZONSOCIAL").toString().isEmpty() )
            result += ", " + getRecProveedora()->getValue("RAZONSOCIAL").toString();
    } else
        result = dbRecord::toString(format, includedFields);
    return result;
}

/*<<<<<FACTURACOMPRA_SAVE*/
bool RecFacturaCompra::save(bool validate, bool saverelated = true)/* throw( dbError ) */
{
/*>>>>>FACTURACOMPRA_SAVE*/
	if( getValue( "CONTADOR" ).toInt() == 0 )
		setValue( "CONTADOR", empresa::ModuleInstance->getMaxContador() );
	if( saverelated ) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) 
			actRestoFactura();
#endif
	}
    bool ret = dbRecord::save(validate, saverelated);
    if( ret && saverelated ) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) {
			delPagos( false );
			genPagos();
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
        DBAPP->showStickyOSD( toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
			Xtring::printf( _("Contador: %d"), getValue( "CONTADOR" ).toInt() ) );
    }
    return ret;
}

/*<<<<<FACTURACOMPRA_REMOVE*/
bool RecFacturaCompra::remove()/* throw( dbError ) */
{
/*>>>>>FACTURACOMPRA_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) 
			delPagos( true );
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
