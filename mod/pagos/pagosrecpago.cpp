/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de pagos
// Member init
// Member save
// Member remove
// Relation empresa::Moneda
// INTERFACE public tesoreria::IApuntableRecord #ifdef|HAVE_TESORERIAMODULE
// TYPE dbRecord pagos::Pago
/*>>>>>MODULE_INFO*/

#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
/*<<<<<PAGO_INCLUDES*/
#include "pagosrecpago.h"
/*>>>>>PAGO_INCLUDES*/
#include "pagosipagablerecord.h"
#include <empresamodule.h>
#include <pagosmodule.h>

namespace gong {
namespace pagos {

/*<<<<<PAGO_INIT*/
void RecPago::init()
{
/*>>>>>PAGO_INIT*/
    addStructuralFilter( "PAGO.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

/*<<<<<PAGO_RELATIONS*/
empresa::RecMoneda *RecPago::getRecMoneda() const
{
	return static_cast<empresa::RecMoneda*>(findRelatedRecord("MONEDA_ID"));
}

/*>>>>>PAGO_RELATIONS*/

#ifdef HAVE_CONTABMODULE
contab::RecCuentaPago *RecPago::getRecCuentaPago() const
{
    return static_cast<contab::RecCuentaPago*>(findRelatedRecord("CUENTAPAGO_ID"));
}
#endif

#ifdef HAVE_TESORERIAMODULE
tesoreria::RecCuentaTesoreria *RecPago::getRecCuentaPago() const
{
    return static_cast<tesoreria::RecCuentaTesoreria*>(findRelatedRecord("CUENTAPAGO_ID"));
}
#endif

dbRecord *RecPago::getRecFactura()
{
    Xtring tablafacturas = getValue( "TABLAFACTURAS" ).toString();
    if( !tablafacturas.isEmpty() ) {
        if( pRecFactura != 0 ) {
            if( pRecFactura->getTableName() != tablafacturas ) {
                delete pRecFactura;
                pRecFactura = 0;
            }
        }
        if( pRecFactura == 0 )
            pRecFactura = DBAPP->createRecord( tablafacturas, 0, getUser() );
        if( getValue( "FACTURA_ID" ).toUInt() != pRecFactura->getRecordID() ) {
            if( !pRecFactura->isRead() )
                pRecFactura->read( getValue( "FACTURA_ID" ).toInt() );
        }
    } else {
        _GONG_DEBUG_WARNING( "PAGO.TABLAFACTURAS está vacío" );
    }
    return pRecFactura;
}

dbRecord *RecPago::getRecTercero()
{
    Xtring tablaterceros = getValue( "TABLATERCEROS" ).toString();
    if( !tablaterceros.isEmpty() ) {
        if( pRecTercero != 0 ) {
            if( pRecTercero->getTableName() != tablaterceros ) {
                delete pRecTercero;
                pRecTercero = 0;
            }
        }
        if( pRecTercero == 0 )
            pRecTercero = DBAPP->createRecord( tablaterceros, 0, getUser() );
        if( getValue( "TERCERO_ID" ).toUInt() != pRecTercero->getRecordID() ) {
            if( !pRecTercero->isRead() )
                pRecTercero->read( getValue( "TERCERO_ID" ).toInt() );
        }
    } else {
        _GONG_DEBUG_WARNING( "PAGO.TABLATERCEROS está vacío" );
    }
    return pRecTercero;
}

/*<<<<<PAGO_SAVE*/
bool RecPago::save(bool validate, bool saverelated = true)/* throw( dbError ) */
{
/*>>>>>PAGO_SAVE*/
	if( getValue( "CONTADOR" ).toInt() == 0 )
		setValue( "CONTADOR", empresa::ModuleInstance->getMaxContador() );
    bool ret = dbRecord::save(validate, saverelated);
    if( ret && saverelated) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) 
			actPagosFactura();
#endif
		DBAPP->showStickyOSD( toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
							  Xtring::printf( _("Contador: %d"), getValue( "CONTADOR" ).toInt() ) );
	}
    return ret;
}

/*<<<<<PAGO_REMOVE*/
bool RecPago::remove()/* throw( dbError ) */
{
/*>>>>>PAGO_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) 
			actPagosFactura();
#endif
	}
    return ret;
}

bool RecPago::actPagosFactura()
{
    if( IPagableRecord *pr = dynamic_cast<IPagableRecord *>(getRecFactura() ) ) {
        return pr->actPagos();
    }
    return false;
}


} // namespace pagos
} // namespace gong

