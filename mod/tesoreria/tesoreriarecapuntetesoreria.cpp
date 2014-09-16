/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de apuntes de tesorería
// MEMBER init
// MEMBER save
// MEMBER remove
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
		if( getValue( "CONCEPTO_ID" ).toUInt() != pRecConcepto->getRecordID() ) {
			if( !pRecConcepto->isRead() ) 
				pRecConcepto->read( getValue( "CONCEPTO_ID" ).toInt() );
		}
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
		if( getValue( "TERCERO_ID" ).toUInt() != pRecTercero->getRecordID() ) {
			if( !pRecTercero->isRead() )
				pRecTercero->read( getValue( "TERCERO_ID" ).toInt() );
		}
    }
    return pRecTercero;
}

/*<<<<<APUNTETESORERIA_SAVE*/
bool RecApunteTesoreria::save(bool saverelated) throw( dbError )
{
/*>>>>>APUNTETESORERIA_SAVE*/
	bool ret = dbRecord::save(saverelated);
	if( ret ) {
		actualizaSaldos(true);
	}
	return ret;
}

/*<<<<<APUNTETESORERIA_REMOVE*/
bool RecApunteTesoreria::remove() throw( dbError )
{
/*>>>>>APUNTETESORERIA_REMOVE*/
	bool ret = dbRecord::remove();
	if( ret ) {
		actualizaSaldos(false);
	}
	return ret;
}

void RecApunteTesoreria::actualizaSaldos(bool saving)
{
	dbRecordID ct_id = getValue("CUENTATESORERIA_ID").toInt();
	bool cargo = getValue("CARGO").toBool();
	Money importe = getValue("IMPORTE").toMoney();
	actSaldoCuenta(ct_id, cargo, importe, saving);
	// Si el tercero es otra cuenta de tesorería, actualizar su saldo
	Xtring tabla_terceros = getValue("TABLATERCEROS").toString();
	if( tabla_terceros == "CUENTATESORERIA" ) 
		actSaldoCuenta( getValue("TERCERO_ID").toInt(), !cargo, importe, saving);
}

/**
 * @brief Actualiza el saldo de la cuenta de tesorería afectada por este apunte
 */
void RecApunteTesoreria::actSaldoCuenta(dbRecordID cuentatesoreria_id, bool cargo, const Money& _importe, bool saving)
{
	Money importe = saving ? _importe : -_importe; 
	RecCuentaTesoreria *cuentatesoreria = static_cast<RecCuentaTesoreria*>( DBAPP->createRecord("CUENTATESORERIA") );
    if ( cuentatesoreria->read( cuentatesoreria_id ) ) {
		if( cargo ) {
			cuentatesoreria->setValue( "HABER", cuentatesoreria->getValue( "HABER" ).toMoney() + importe );
			cuentatesoreria->setValue( "SALDO", cuentatesoreria->getValue( "SALDO" ).toMoney() - importe );
		} else {
			cuentatesoreria->setValue( "DEBE", cuentatesoreria->getValue( "DEBE" ).toMoney() + importe );
			cuentatesoreria->setValue( "SALDO", cuentatesoreria->getValue( "SALDO" ).toMoney() + importe );
		}
        cuentatesoreria->save(false);
    }
    delete cuentatesoreria;
    // Actualizar saldo cuenta
}


} // namespace tesoreria
} // namespace gong
