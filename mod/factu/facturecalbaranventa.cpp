/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de albaranes de venta
// MEMBER init
// MEMBER save
// MEMBER remove
// MEMBER toString
// RELATION TipoDoc
// RELATION Cliente
// RELATION Agente
// RELATION pagos::FormaPago
// RELATION empresa::Proyecto
// RELATION AlbaranVentaDet Detalles
// INTERFACE public factu::IPagableAlbaran
// INTERFACE public factu::IIVADesglosable
// INTERFACE public factu::IAsentableFactura #ifdef|HAVE_CONTABMODULE
// INTERFACE public factu::ITotalizableRecord
// TYPE dbRecord factu::AlbaranVenta
/*>>>>>MODULE_INFO*/

#include "config.h"
/*<<<<<ALBARANVENTA_INCLUDES*/
#include "facturecalbaranventa.h"
/*>>>>>ALBARANVENTA_INCLUDES*/
#include <empresamodule.h>
#include <factumodule.h>
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#include <contabrecasiento.h>
#endif

namespace gong {
namespace factu {

/*<<<<<ALBARANVENTA_INIT*/
void RecAlbaranVenta::init()
{
/*>>>>>ALBARANVENTA_INIT*/
    addStructuralFilter( "ALBARANVENTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "ALBARANVENTA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

/*<<<<<ALBARANVENTA_RELATIONS*/
RecTipoDoc *RecAlbaranVenta::getRecTipoDoc() const
{
	return static_cast<RecTipoDoc*>(findRelatedRecord("TIPODOC_ID"));
}

RecCliente *RecAlbaranVenta::getRecCliente() const
{
	return static_cast<RecCliente*>(findRelatedRecord("CLIENTE_ID"));
}

RecAgente *RecAlbaranVenta::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("AGENTE_ID"));
}

pagos::RecFormaPago *RecAlbaranVenta::getRecFormaPago() const
{
	return static_cast<pagos::RecFormaPago*>(findRelatedRecord("FORMAPAGO_ID"));
}

empresa::RecProyecto *RecAlbaranVenta::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

RecAlbaranVentaDet *RecAlbaranVenta::getRecAlbaranVentaDet( int albaranventadet ) const
{
	return static_cast<RecAlbaranVentaDet*>(findRelationByRelatedTable("ALBARANVENTADET" )->getRelatedRecord( albaranventadet));
}

dbRecordList *RecAlbaranVenta::getListAlbaranVentaDet() const
{
	return findRelationByRelatedTable( "ALBARANVENTADET" )->getRelatedRecordList();
}
/*>>>>>ALBARANVENTA_RELATIONS*/

/*<<<<<ALBARANVENTA_TOSTRING*/
Xtring RecAlbaranVenta::toString(int format, const Xtring &includedFields) const
{
	Xtring result;
/*>>>>>ALBARANVENTA_TOSTRING*/
    if( format == TOSTRING_CODE_AND_DESC_WITH_TABLENAME ) {
        result = dbRecord::toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME );
        if( !getRecCliente()->getValue("RAZONSOCIAL").toString().isEmpty() )
            result += ", " + getRecCliente()->getValue("RAZONSOCIAL").toString();
    } else
        result = dbRecord::toString(format, includedFields);
    return result;
}

/*<<<<<ALBARANVENTA_SAVE*/
bool RecAlbaranVenta::save(bool saverelated) throw( dbError )
{
/*>>>>>ALBARANVENTA_SAVE*/
#ifdef HAVE_PAGOSMODULE
    actRestoFactura();
#endif
    bool ret = dbRecord::save(saverelated);
    if( ret && saverelated ) {
#ifdef HAVE_PAGOSMODULE
        delCobros( false );
        genCobros();
#endif
#ifdef HAVE_CONTABMODULE
        if( contab::ModuleInstance->isContabActive() ) {
            bool supervisar = contab::ModuleInstance->getModuleSetting( "SUPERVISAR_ASIENTOS" ).toBool();
            regenAsiento( supervisar );
        }
#endif
    }
    return ret;
}

/*<<<<<ALBARANVENTA_REMOVE*/
bool RecAlbaranVenta::remove() throw( dbError )
{
/*>>>>>ALBARANVENTA_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
        delCobros( true );
#endif
#ifdef HAVE_CONTABMODULE
        if( contab::ModuleInstance->isContabActive() )
            delete borraAsiento();
#endif
    }
    return ret;
}

void RecAlbaranVenta::setNextNumero( bool checkdup )
{
    int contador = ModuleInstance->getMaxContador( "ALBARANVENTA", getValue("EMPRESA_ID").toInt(),
                   ModuleInstance->getEmpresaModule()->getEjercicio(),
                   getRecTipoDoc()->getValue("SERIE").toString() );
    while( 1 ) {
        setValue( "CONTADOR", contador );
        setValue( "NUMERO", factu::RecTipoDoc::formatNumDocumento(
                      empresa::ModuleInstance->getRecEmpresa()->getValue( "CODIGO" ).toInt(),
                      empresa::ModuleInstance->getEjercicio(),
                      contador, getRecAgente()->getValue("CODIGO").toInt(),
                      getRecTipoDoc()->getValue("FORMATO").toString()) );
        if( !checkdup )
            return;
        // Comprobar que no exista el número que se ha generado automáticamente,
        if( existsAnother( "NUMERO=" + getConnection()->toSQL( getValue( "NUMERO" ).toString() ) ) ) {
            contador++;
        } else {
            break;
        }
    }
}

} // namespace factu
} // namespace gong
