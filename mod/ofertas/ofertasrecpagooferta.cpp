/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de pagos de ofertas
// Member init
// Member save
// Member remove
// Relation empresa::Moneda
// INTERFACE public tesoreria::IApuntableRecord #ifdef|HAVE_TESORERIAMODULE
// TYPE dbRecord ofertas::PagoOferta
/*>>>>>MODULE_INFO*/

#include "ofertasmodule.h"
/*<<<<<PAGOOFERTA_INCLUDES*/
#include "ofertasrecpagooferta.h"
/*>>>>>PAGOOFERTA_INCLUDES*/
#include "pagosipagablerecord.h"

namespace gong {
namespace ofertas {

/*<<<<<PAGOOFERTA_INIT*/
void RecPagoOferta::init()
{
/*>>>>>PAGOOFERTA_INIT*/
    addStructuralFilter( "PAGOOFERTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

/*<<<<<PAGOOFERTA_RELATIONS*/
empresa::RecMoneda *RecPagoOferta::getRecMoneda() const
{
	return static_cast<empresa::RecMoneda*>(findRelatedRecord("MONEDA_ID"));
}

/*>>>>>PAGOOFERTA_RELATIONS*/

/*<<<<<PAGOOFERTA_SAVE*/
bool RecPagoOferta::save(bool validate, bool saverelated = true)/* throw( dbError ) */
{
/*>>>>>PAGOOFERTA_SAVE*/
    if( getValue( "CONTADOR" ).toInt() == 0 ) {
        setValue( "CONTADOR", ModuleInstance->getMaxContador() );
	}
	bool ret = dbRecord::save(validate, saverelated);
	if (ret) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) 
			actPagosFactura();
#endif	
		DBAPP->showStickyOSD( toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
							  Xtring::printf( _("Contador: %d"), getValue( "CONTADOR" ).toInt() ) );
	}
	return ret;
}

/*<<<<<PAGOOFERTA_REMOVE*/
bool RecPagoOferta::remove()/* throw( dbError ) */
{
/*>>>>>PAGOOFERTA_REMOVE*/
	bool ret = dbRecord::remove();
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) 
			actPagosFactura();
#endif
	}
	return ret;
}

bool RecPagoOferta::actPagosFactura()
{
	if( pagos::IPagableRecord *pr = dynamic_cast<pagos::IPagableRecord *>(getRecFactura() ) ) {
		return pr->actPagos();
	}
	return false;
}

dbRecord *RecPagoOferta::getRecFactura()
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

dbRecord *RecPagoOferta::getRecTercero()
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
        _GONG_DEBUG_WARNING( "PAGOOFERTA.TABLATERCEROS está vacío" );
    }
    return pRecTercero;
}



} // namespace ofertas
} // namespace gong
