/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de cobros
// Member init
// Member save
// Member remove
// Relation empresa::Moneda
// INTERFACE public tesoreria::IApuntableRecord #ifdef|HAVE_TESORERIAMODULE
// TYPE dbRecord ofertas::CobroOferta
/*>>>>>MODULE_INFO*/

#include <pagosipagablerecord.h>
#include "ofertasmodule.h"
/*<<<<<COBROOFERTA_INCLUDES*/
#include "ofertasreccobrooferta.h"
/*>>>>>COBROOFERTA_INCLUDES*/

namespace gong {
namespace ofertas {

/*<<<<<COBROOFERTA_INIT*/
void RecCobroOferta::init()
{
/*>>>>>COBROOFERTA_INIT*/
    addStructuralFilter( "COBROOFERTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

/*<<<<<COBROOFERTA_RELATIONS*/
empresa::RecMoneda *RecCobroOferta::getRecMoneda() const
{
	return static_cast<empresa::RecMoneda*>(findRelatedRecord("MONEDA_ID"));
}

/*>>>>>COBROOFERTA_RELATIONS*/

/*<<<<<COBROOFERTA_SAVE*/
bool RecCobroOferta::save(bool validate, bool saverelated = true)/* throw( dbError ) */
{
/*>>>>>COBROOFERTA_SAVE*/
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

/*<<<<<COBROOFERTA_REMOVE*/
bool RecCobroOferta::remove()/* throw( dbError ) */
{
/*>>>>>COBROOFERTA_REMOVE*/
	bool ret = dbRecord::remove();
	if (ret) {
#ifdef HAVE_PAGOSMODULE
		if( DBAPP->findModule("pagos") ) {
			actPagosFactura();
		}
#endif
	}
	return ret;
}


bool RecCobroOferta::actPagosFactura()
{
	if( pagos::IPagableRecord *pr = dynamic_cast<pagos::IPagableRecord *>(getRecFactura() ) ) {
		return pr->actPagos();
	}
	return false;
}


dbRecord *RecCobroOferta::getRecFactura()
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
        _GONG_DEBUG_WARNING( "COBROOFERTA.TABLAFACTURAS está vacío" );
    }
    return pRecFactura;
}

dbRecord *RecCobroOferta::getRecTercero()
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
        _GONG_DEBUG_WARNING( "COBROOFERTA.TABLATERCEROS está vacío" );
    }
    return pRecTercero;
}


} // namespace ofertas
} // namespace gong
