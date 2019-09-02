/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de apuntes de ofertas
// MEMBER init
// MEMBER save
// MEMBER remove
// RELATION tesoreria::TipoApunteTesoreria #ifdef|HAVE_TESORERIAMODULE
// RELATION CuentaOferta
// RELATION empresa::Proyecto
// TYPE dbRecord ofertas::ApunteOferta
/*>>>>>MODULE_INFO*/

#include <empresamodule.h>
/*<<<<<APUNTEOFERTA_INCLUDES*/
#include "ofertasrecapunteoferta.h"
/*>>>>>APUNTEOFERTA_INCLUDES*/

namespace gong {
namespace ofertas {


/*<<<<<APUNTEOFERTA_INIT*/
void RecApunteOferta::init()
{
/*>>>>>APUNTEOFERTA_INIT*/
    addStructuralFilter( "APUNTEOFERTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

/*<<<<<APUNTEOFERTA_RELATIONS*/
#ifdef HAVE_TESORERIAMODULE
tesoreria::RecTipoApunteTesoreria *RecApunteOferta::getRecTipoApunteTesoreria() const
{
	return static_cast<tesoreria::RecTipoApunteTesoreria*>(findRelatedRecord("TIPOAPUNTETESORERIA_ID"));
}

#endif
RecCuentaOferta *RecApunteOferta::getRecCuentaOferta() const
{
	return static_cast<RecCuentaOferta*>(findRelatedRecord("CUENTAOFERTA_ID"));
}

empresa::RecProyecto *RecApunteOferta::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("PROYECTO_ID"));
}

/*>>>>>APUNTEOFERTA_RELATIONS*/


RecConcepto *RecApunteOferta::getRecConcepto()
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

RecTercero *RecApunteOferta::getRecTercero()
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

/*<<<<<APUNTEOFERTA_SAVE*/
bool RecApunteOferta::save(bool validate, bool saverelated = true)/* throw( dbError ) */
{
/*>>>>>APUNTEOFERTA_SAVE*/
	bool wasnew = isNew();
    bool ret = dbRecord::save(saverelated);
    if( ret ) {
		// Descontar el original
		Money importe = getOrigValue("IMPORTE").toDouble();
		if( importe != 0.0 ) {
			dbRecordID ct_id = getOrigValue("CUENTAOFERTA_ID").toInt();
			actSaldoCuenta(ct_id, importe, false);
		}
		importe = getValue("IMPORTE").toDouble();
		if( importe != 0.0 ) {
			dbRecordID ct_id = getValue("CUENTAOFERTA_ID").toInt();
			actSaldoCuenta(ct_id, importe, true);
		}
		if( !wasnew ) {
			if( generaContrapartida() )	{
				DBAPP->showStickyOSD( toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
					"Se ha generado la correspondiente contrapartida" );
			}
		}
	}
    return ret;
}

/*<<<<<APUNTEOFERTA_REMOVE*/
bool RecApunteOferta::remove()/* throw( dbError ) */
{
/*>>>>>APUNTEOFERTA_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
		Money importe = getValue("IMPORTE").toDouble();
		if( importe != 0.0 ) {
			dbRecordID ct_id = getValue("CUENTAOFERTA_ID").toInt();
			actSaldoCuenta(ct_id, importe, false);
		}
    }
    return ret;
}

/**
 * @brief Actualiza el saldo de la cuenta de ofertas afectada por este apunte
 */
void RecApunteOferta::actSaldoCuenta(dbRecordID cuentaoferta_id, const Money& _importe, bool saving)
{
    Money importe = saving ? _importe : -_importe;
    RecCuentaOferta *cuentaoferta = static_cast<RecCuentaOferta*>( DBAPP->createRecord("CUENTAOFERTA") );
    if ( cuentaoferta->read( cuentaoferta_id ) ) {
		cuentaoferta->setValue( "SALDO", cuentaoferta->getValue( "SALDO" ).toMoney() + importe );
        cuentaoferta->save(false);
    }
    delete cuentaoferta;
}


/**
 * @brief Genera un apunte contrapartida del actual, solo si no es automático, si el tercero es una cuenta de ofertas y
 * si no está ya creado.
 *
 * @return bool
 */

bool RecApunteOferta::generaContrapartida()
{
	if( getValue("TABLATERCEROS").toString() != "CUENTAOFERTA" || getValue("AUTOMATICO").toBool() )
		return false;
	dbConnection *conn = getConnection();
	RecApunteOferta *apunte = static_cast<RecApunteOferta*>(duplicate());
	if( apunte->read(
		Xtring("EJERCICIO=") + conn->toSQL( getValue("EJERCICIO") )
		+ " AND NUMERO=" + conn->toSQL( getValue("NUMERO") )
		+ " AND FECHA=" + conn->toSQL( getValue("FECHA") )
		+ " AND IMPORTE=" + conn->toSQL( getValue("IMPORTE").toMoney() )
		+ " AND PROYECTO_ID=" + conn->toSQL( getValue("PROYECTO_ID") )
		+ " AND CUENTAOFERTA_ID=" + conn->toSQL( getValue("TERCERO_ID") )
		+ " AND TABLATERCEROS=" + conn->toSQL("CUENTAOFERTA")
		+ " AND TERCERO_ID=" + conn->toSQL( getValue("CUENTAOFERTA_ID") )
		+ " AND TABLACONCEPTOS=" + conn->toSQL( getValue("TABLACONCEPTOS") )
		+ " AND CONCEPTO_ID=" + conn->toSQL( getValue("CONCEPTO_ID") )
		+ " AND CONCEPTO=" + conn->toSQL( getValue("CONCEPTO") )
		+ " AND REFERENCIA=" + conn->toSQL( getValue("REFERENCIA") ) ) ) {
		return false;
	} else {
		apunte->copyRecord( this );
		apunte->setNew(true);
		apunte->setValue( "IMPORTE", -getValue("IMPORTE").toMoney() );
		apunte->setValue( "CUENTAOFERTA_ID", getValue("TERCERO_ID") );
		apunte->setValue( "TERCERO_ID", getValue("CUENTAOFERTA_ID") );
		return apunte->save(false);
	}

}

} // namespace ofertas
} // namespace gong
