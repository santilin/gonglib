/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de cobros
// Member init
// Member save
// Member remove
// Relation empresa::Moneda
// Relation RemesaCobro
// TYPE dbRecord pagos::Cobro
/*>>>>>MODULE_INFO*/

#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
/*<<<<<COBRO_INCLUDES*/
#include "pagosreccobro.h"
/*>>>>>COBRO_INCLUDES*/
#include "pagosipagablerecord.h"
#include <empresamodule.h>
#include <pagosmodule.h>

namespace gong {
namespace pagos {

/*<<<<<COBRO_INIT*/
void RecCobro::init()
{
/*>>>>>COBRO_INIT*/
    addStructuralFilter( "COBRO.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

/*<<<<<COBRO_RELATIONS*/
empresa::RecMoneda *RecCobro::getRecMoneda() const
{
	return static_cast<empresa::RecMoneda*>(findRelatedRecord("MONEDA_ID"));
}

RecRemesaCobro *RecCobro::getRecRemesaCobro() const
{
	return static_cast<RecRemesaCobro*>(findRelatedRecord("REMESACOBRO_ID"));
}

/*>>>>>COBRO_RELATIONS*/

#ifdef HAVE_CONTABMODULE
contab::RecCuentaPago *RecCobro::getRecCuentaPago() const
{
    return static_cast<contab::RecCuentaPago*>(findRelatedRecord("CUENTAPAGO_ID"));
}
#endif

/*<<<<<COBRO_SAVE*/
bool RecCobro::save(bool saverelated) throw( dbError )
{
/*>>>>>COBRO_SAVE*/
    if( getValue( "CONTADOR" ).toInt() == 0 )
        setValue( "CONTADOR", empresa::ModuleInstance->getMaxContador() );
    bool ret = dbRecord::save(saverelated);
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
        actCobrosFactura();
#endif
    }
    if( ret )
        DBAPP->showStickyOSD( toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
                              Xtring::printf( _("Contador: %d"), getValue( "CONTADOR" ).toInt() ) );
    return ret;
}

/*<<<<<COBRO_REMOVE*/
bool RecCobro::remove() throw( dbError )
{
/*>>>>>COBRO_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
#ifdef HAVE_PAGOSMODULE
        actCobrosFactura();
#endif
    }
    return ret;
}

dbRecord *RecCobro::getRecFactura()
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
        if( getValue( "FACTURA_ID" ).toUInt() != 0
                && (getValue( "FACTURA_ID" ).toUInt() != pRecFactura->getRecordID() ) )
            pRecFactura->read( getValue( "FACTURA_ID" ).toInt() );
    } else {
        _GONG_DEBUG_WARNING( "COBRO.TABLAFACTURAS está vacío" );
    }
    return pRecFactura;
}

dbRecord *RecCobro::getRecTercero()
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
        if( getValue( "TERCERO_ID" ).toUInt() != 0
                && (getValue( "TERCERO_ID" ).toUInt() != pRecTercero->getRecordID()) )
            pRecTercero->read( getValue( "TERCERO_ID" ).toInt() );
    } else {
        _GONG_DEBUG_WARNING( "COBRO.TABLATERCEROS está vacío" );
    }
    return pRecTercero;
}

bool RecCobro::actCobrosFactura()
{
    if( IPagableRecord *pr = dynamic_cast<IPagableRecord *>(getRecFactura() ) ) {
        return pr->actPagos();
    }
    return false;
}

Xtring RecCobro::formatNumRecibo(int codempresa, int ejercicio, int remesa,
                                 int numero, int proyecto, const Xtring& proyname,
                                 const Xtring& formato)
{
    if( formato.isEmpty() )
        return Xtring::number( numero );
    else {
        Xtring result;
        for( uint i = 0; i < formato.size(); ++ i ) {
            if( formato[i] == '{' ) {
                ++i;
                Xtring fieldandsize, field, size;
                while( formato[i] != '}' && i < formato.size() )
                    fieldandsize += formato[i++];
                fieldandsize.splitIn2( field, size, ":" );
                Xtring bit;
                if( field == "eje" || field == "ejercicio" ) {
                    bit = Xtring::number( ejercicio );
                } else if( field == "emp" || field == "empresa" || field == "codemp" ) {
                    bit = Xtring::number( codempresa );
                } else if( field == "num"  ) {
                    bit = Xtring::number( numero );
                } else if( field == "rem" || field == "remesa"  ) {
                    bit = Xtring::number( remesa );
                } else if( field == "proy" || field == "proyecto" ) {
                    bit = Xtring::number( proyecto );
                } else if( field == "nombreproyecto" || field == "nomproy" ) {
                    bit = proyname;
                } else {
                    _GONG_DEBUG_WARNING( "Field " + field + " not found" );
                    result += fieldandsize;
                    continue;
                }
                if( size.toInt() != 0 )
                    bit.padL( size.toInt(), '0' );
                result += bit;
            } else {
                result += formato[i];
            }
        }
        return result;
    }
}

} // namespace pagos
} // namespace gong

