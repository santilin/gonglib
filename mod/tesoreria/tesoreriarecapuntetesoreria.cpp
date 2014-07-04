/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de apuntes de tesorería
// MEMBER init
// RELATION CuentaTesoreria
// RELATION TipoApunteTesoreria
// RELATION empresa::Proyecto
// TYPE dbRecord tesoreria::ApunteTesoreria
/*>>>>>MODULE_INFO*/

#include <empresamodule.h>
/*<<<<<APUNTETESORERIA_INCLUDES*/
#include "tesoreriarecapuntetesoreria.h"
/*>>>>>APUNTETESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {

/*<<<<<APUNTETESORERIA_INIT*/
void RecApunteTesoreria::init()
{
/*>>>>>APUNTETESORERIA_INIT*/
    addStructuralFilter( "APUNTETESORERIA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

/*<<<<<APUNTETESORERIA_RELATIONS*/
RecCuentaTesoreria *RecApunteTesoreria::getRecCuentaTesoreria() const
{
	return static_cast<RecCuentaTesoreria*>(findRelatedRecord("CUENTATESORERIA_ID"));
}

RecTipoApunteTesoreria *RecApunteTesoreria::getRecTipoApunteTesoreria() const
{
	return static_cast<RecTipoApunteTesoreria*>(findRelatedRecord("TIPOAPUNTETESORERIA_ID"));
}

empresa::RecProyecto *RecApunteTesoreria::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

/*>>>>>APUNTETESORERIA_RELATIONS*/


RecConcepto *RecApunteTesoreria::getRecConcepto()
{
    Xtring tablaconceptos = getValue( "TABLACONCEPTOS" ).toString();
    if( !tablaconceptos.isEmpty() ) {
        if( pRecConcepto != 0 ) {
            if( pRecConcepto->getTableName() != tablaconceptos ) {
                delete pRecConcepto;
                pRecConcepto = 0;
            }
        }
        if( pRecConcepto == 0 )
            pRecConcepto = DBAPP->createRecord( tablaconceptos, 0, getUser() );
//        if( getValue( "CONCEPTO_ID" ).toUInt() != pRecConcepto->getRecordID() )
//            pRecConcepto->read( getValue( "CONCEPTO_ID" ).toInt() );
    }
    return pRecConcepto;
}

RecTercero *RecApunteTesoreria::getRecTercero()
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
//        if( getValue( "TERCERO_ID" ).toUInt() != pRecTercero->getRecordID() )
//            pRecTercero->read( getValue( "TERCERO_ID" ).toInt() );
    }
    return pRecTercero;
}


} // namespace tesoreria
} // namespace gong
