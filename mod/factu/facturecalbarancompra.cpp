/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de albaranes de compra
// MEMBER init
// MEMBER save
// MEMBER remove
// MEMBER toString
// RELATION TipoDoc
// RELATION Proveedora
// RELATION Agente
// RELATION pagos::FormaPago
// RELATION empresa::Proyecto
// RELATION AlbaranCompraDet Detalles
// INTERFACE public factu::IPagableAlbaran
// INTERFACE public factu::IIVADesglosable
// INTERFACE public factu::IAsentableFactura #ifdef|HAVE_CONTABMODULE
// INTERFACE public tesoreria::IApuntableRecord #ifdef|HAVE_TESORERIAMODULE
// INTERFACE public factu::ITotalizableRecord
// TYPE dbRecord factu::AlbaranCompra
/*>>>>>MODULE_INFO*/

#include "config.h"
/*<<<<<ALBARANCOMPRA_INCLUDES*/
#include "facturecalbarancompra.h"
/*>>>>>ALBARANCOMPRA_INCLUDES*/
#include <empresamodule.h>
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#include <contabrecasiento.h>
#endif
#include "factumodule.h"

namespace gong {
namespace factu {

/*<<<<<ALBARANCOMPRA_INIT*/
void RecAlbaranCompra::init()
{
/*>>>>>ALBARANCOMPRA_INIT*/
    addStructuralFilter( "ALBARANCOMPRA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "ALBARANCOMPRA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

/*<<<<<ALBARANCOMPRA_RELATIONS*/
RecTipoDoc *RecAlbaranCompra::getRecTipoDoc() const
{
	return static_cast<RecTipoDoc*>(findRelatedRecord("TIPODOC_ID"));
}

RecProveedora *RecAlbaranCompra::getRecProveedora() const
{
	return static_cast<RecProveedora*>(findRelatedRecord("PROVEEDORA_ID"));
}

RecAgente *RecAlbaranCompra::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("AGENTE_ID"));
}

pagos::RecFormaPago *RecAlbaranCompra::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("FORMAPAGO_ID"));
}

empresa::RecProyecto *RecAlbaranCompra::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

RecAlbaranCompraDet *RecAlbaranCompra::getRecAlbaranCompraDet( int albarancompradet ) const
{
	return static_cast<RecAlbaranCompraDet*>(findRelationByRelatedTable("ALBARANCOMPRADET" )->getRelatedRecord( albarancompradet));
}

dbRecordList *RecAlbaranCompra::getListAlbaranCompraDet() const
{
	return findRelationByRelatedTable( "ALBARANCOMPRADET" )->getRelatedRecordList();
}
/*>>>>>ALBARANCOMPRA_RELATIONS*/

/*<<<<<ALBARANCOMPRA_TOSTRING*/
Xtring RecAlbaranCompra::toString(int format, const Xtring &includedFields) const
{
	Xtring result;
/*>>>>>ALBARANCOMPRA_TOSTRING*/
    if( format == TOSTRING_CODE_AND_DESC_WITH_TABLENAME || format == TOSTRING_CODE_AND_DESC || format == TOSTRING_DESC ) {
        result = dbRecord::toString( format, includedFields );
        if( !getRecProveedora()->getValue("RAZONSOCIAL").toString().isEmpty() )
            result += ", " + getRecProveedora()->getValue("RAZONSOCIAL").toString();
    } else
        result = dbRecord::toString(format, includedFields);
    return result;
}

/*<<<<<ALBARANCOMPRA_SAVE*/
bool RecAlbaranCompra::save(bool saverelated) throw( dbError )
{
/*>>>>>ALBARANCOMPRA_SAVE*/
	if( getValue( "CONTADOR" ).toInt() == 0 )
		setValue( "CONTADOR", empresa::ModuleInstance->getMaxContador() );
	if( saverelated ) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) 
			actRestoFactura();
#endif
	}
    bool ret = dbRecord::save(saverelated);
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

/*<<<<<ALBARANCOMPRA_REMOVE*/
bool RecAlbaranCompra::remove() throw( dbError )
{
/*>>>>>ALBARANCOMPRA_REMOVE*/
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
