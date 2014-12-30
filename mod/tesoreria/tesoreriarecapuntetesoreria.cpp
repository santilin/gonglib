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
	bool wasnew = isNew();
    bool ret = dbRecord::save(saverelated);
    if( ret ) {
		// Descontar el original
		Money debe = getOrigValue("DEBE").toDouble();
		Money haber = getOrigValue("HABER").toDouble();
		if( debe != 0.0 || haber != 0.0 ) {
			dbRecordID ct_id = getOrigValue("CUENTATESORERIA_ID").toInt();
			actSaldoCuenta(ct_id, debe, haber, false); 
		}
		debe = getValue("DEBE").toDouble();
		haber = getValue("HABER").toDouble();
		if( debe != 0.0 || haber != 0.0 ) {
			dbRecordID ct_id = getValue("CUENTATESORERIA_ID").toInt();
			actSaldoCuenta(ct_id, debe, haber, true); 
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

/*<<<<<APUNTETESORERIA_REMOVE*/
bool RecApunteTesoreria::remove() throw( dbError )
{
/*>>>>>APUNTETESORERIA_REMOVE*/
    bool ret = dbRecord::remove();
    if( ret ) {
		Money debe = getValue("DEBE").toDouble();
		Money haber = getValue("HABER").toDouble();
		if( debe != 0.0 || haber != 0.0 ) {
			dbRecordID ct_id = getValue("CUENTATESORERIA_ID").toInt();
			actSaldoCuenta(ct_id, debe, haber, false); 
		}
    }
    return ret;
}

/**
 * @brief Actualiza el saldo de la cuenta de tesorería afectada por este apunte
 */
void RecApunteTesoreria::actSaldoCuenta(dbRecordID cuentatesoreria_id, const Money& _debe, const Money &_haber, bool saving)
{
    Money debe = saving ? _debe : -_debe;
    Money haber = saving ? _haber: -_haber;
    RecCuentaTesoreria *cuentatesoreria = static_cast<RecCuentaTesoreria*>( DBAPP->createRecord("CUENTATESORERIA") );
    if ( cuentatesoreria->read( cuentatesoreria_id ) ) {
		cuentatesoreria->setValue( "DEBE", cuentatesoreria->getValue( "DEBE" ).toMoney() + debe );
		cuentatesoreria->setValue( "HABER", cuentatesoreria->getValue( "HABER" ).toMoney() + haber );
		cuentatesoreria->setValue( "SALDO", cuentatesoreria->getValue( "SALDO" ).toMoney() + debe - haber );
        cuentatesoreria->save(false);
    }
    delete cuentatesoreria;
}


/**
 * @brief Genera un apunte contrapartida del actual, solo si no es automático, si el tercero es una cuenta de tesorería y 
 * si no está ya creado.
 * 
 * @return bool
 */

bool RecApunteTesoreria::generaContrapartida()
{
	if( getValue("TABLATERCEROS").toString() != "CUENTATESORERIA" || getValue("AUTOMATICO").toBool() ) 
		return false;
	dbConnection *conn = getConnection();
	RecApunteTesoreria *apunte = static_cast<RecApunteTesoreria*>(duplicate());
	if( apunte->read( 
		Xtring("EJERCICIO=") + conn->toSQL( getValue("EJERCICIO") ) 
		+ " AND NUMERO=" + conn->toSQL( getValue("NUMERO") )
		+ " AND FECHA=" + conn->toSQL( getValue("FECHA") )
		+ " AND DEBE=" + conn->toSQL( getValue("DEBE").toMoney() )
		+ " AND HABER=" + conn->toSQL( getValue("HABER").toMoney() )
		+ " AND PROYECTO_ID=" + conn->toSQL( getValue("PROYECTO_ID") )
		+ " AND CUENTATESORERIA_ID=" + conn->toSQL( getValue("TERCERO_ID") )
		+ " AND TABLATERCEROS=" + conn->toSQL("CUENTATESORERIA") 
		+ " AND TERCERO_ID=" + conn->toSQL( getValue("CUENTATESORERIA_ID") )
		+ " AND TABLACONCEPTOS=" + conn->toSQL( getValue("TABLACONCEPTOS") )
		+ " AND CONCEPTO_ID=" + conn->toSQL( getValue("CONCEPTO_ID") )
		+ " AND CONCEPTO=" + conn->toSQL( getValue("CONCEPTO") )
		+ " AND REFERENCIA=" + conn->toSQL( getValue("REFERENCIA") ) ) ) {
		return false;
	} else {
		apunte->copyRecord( this );
		apunte->setNew(true);
		apunte->setValue( "DEBE", getValue("HABER").toMoney() );
		apunte->setValue( "HABER", getValue("DEBE").toMoney() );
		apunte->setValue( "CUENTATESORERIA_ID", getValue("TERCERO_ID") );
		apunte->setValue( "TERCERO_ID", getValue("CUENTATESORERIA_ID") );
		return apunte->save(false);
	}
		
}

} // namespace tesoreria
} // namespace gong
