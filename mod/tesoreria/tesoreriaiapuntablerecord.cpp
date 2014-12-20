#include <dbappdbapplication.h>
#include <dbappmainwindow.h>
#include <dbappfrmeditrec.h>
#include <empresamodule.h>
#include "tesoreriaiapuntablerecord.h"

namespace gong {
namespace tesoreria {

IApuntableRecord::IApuntableRecord(dbRecord* record, const Xtring& apunte_id_field, const Xtring& cuenta_tesoreria_id_field, 
	const Xtring& fecha_field, const Xtring& importe_field, const Xtring& referencia_field, 
	const Xtring& tablaterceros, bool terceros_is_field, const Xtring& tercero_id_field, const Xtring& tercero_field, 
	const Xtring& tablaconceptos, bool conceptos_is_field, const Xtring& concepto_id_field, const Xtring& concepto_field, 
	const Xtring& notas_field, const Xtring& proyecto_id_field)
	: pRecord(record), mApunteIDField(apunte_id_field), mCuentaTesoreriaIDField(cuenta_tesoreria_id_field), mFechaField(fecha_field), 
		mImporteField(importe_field), mReferenciaField(referencia_field), mTablaTerceros(tablaterceros), 
		mTercerosIsField( terceros_is_field), mTerceroIDField(tercero_id_field), mTerceroField(tercero_field), 
		mTablaConceptos(tablaconceptos), mConceptosIsField(conceptos_is_field),	mConceptoIDField(concepto_id_field), 
		mConceptoField(concepto_field), mNotasField(notas_field), mProyectoIDField(proyecto_id_field)
{
}

RecCuentaTesoreria *IApuntableRecord::getRecCuentaTesoreria() const
{
    return static_cast<RecCuentaTesoreria*>(pRecord->findRelatedRecord(mCuentaTesoreriaIDField));
}

RecApunteTesoreria* IApuntableRecord::borraApunte(bool regenerando)
{
    RecApunteTesoreria *apunte = static_cast<RecApunteTesoreria *>(DBAPP->createRecord( "APUNTETESORERIA" ));
    if( pRecord->getValue( mApunteIDField ).toInt() && apunte->read( pRecord->getValue( mApunteIDField ).toInt() ) ) {
        if (apunte->remove() ) {
			if (!regenerando) {
				DBAPP->showStickyOSD( pRecord->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
							  "Apunte borrado en tesorería");
			}
		}
    }
    pRecord->setValue( mApunteIDField, 0 );
    return apunte;
}

dbRecordID IApuntableRecord::regenApunte(bool supervisar)
{
    dbRecordID ret = 0;
    RecApunteTesoreria *old_apunte = borraApunte( true /*regenerando*/);
    RecApunteTesoreria *apunte = creaApunte( old_apunte, supervisar );
    if( apunte )
        ret = apunte->getRecordID();
    delete apunte;
    delete old_apunte;
    return ret;
}

FrmEditRec *IApuntableRecord::showApunte( DataTable::EditMode editmode)
{
    FrmEditRec *frm = DBAPP->createEditForm(0, pRecord, 0, editmode);
    DBAPP->getMainWindow()->createClient( frm );
    return frm;
}


RecApunteTesoreria* IApuntableRecord::creaApunte(RecApunteTesoreria* old_apunte, bool supervisar)
{
	if( pRecord->getValue(mImporteField).toDouble() == 0.0 )
		return 0;
	dbRecordID cuenta_pago_id = pRecord->getValue(mCuentaTesoreriaIDField).toInt();
	if( cuenta_pago_id == 0 ) {
		FrmBase::msgError( "Tesorería", _("No se ha generado el apunte en tesorería porque no se ha introducido una cuenta de pago"));
		return 0;
	}
	RecApunteTesoreria *apunte = static_cast<RecApunteTesoreria *>(DBAPP->createRecord("APUNTETESORERIA"));
	apunte->setValue( "CUENTATESORERIA_ID", cuenta_pago_id );
	if( old_apunte->isRead() ) // Recycle the id
		apunte->setRecordID( old_apunte->getRecordID() );
	apunte->setValue( "AUTOMATICO", true );
	apunte->setValue( "NUMERO", apunte->selectNextInt( "NUMERO" ) );
	apunte->setValue( "EMPRESA_ID", pRecord->getValue("EMPRESA_ID").toInt() );
	apunte->setValue( "EJERCICIO", pRecord->getValue("EJERCICIO").toInt() );
	apunte->setValue( "FECHA", pRecord->getValue(mFechaField).toDate() );
	apunte->setValue( "DEBE", pRecord->getValue(mImporteField).toMoney() );
	apunte->setValue( "SALDO", pRecord->getValue(mImporteField).toMoney() );
	apunte->setValue( "REFERENCIA", pRecord->getValue(mReferenciaField) );
	_GONG_DEBUG_PRINT(0, apunte->toString(TOSTRING_DEBUG_COMPLETE));
	_GONG_DEBUG_PRINT(0, pRecord->toString(TOSTRING_DEBUG_COMPLETE));
	if( mTablaTerceros == Xtring::null && mTerceroField == Xtring::null ) {
	} else if( mTablaTerceros != Xtring::null ) {
		if( mTercerosIsField ) {
			// No hay relación directa entre pRecord(Pago) y la tabla de terceros.
			Xtring tablaterceros = pRecord->getValue(mTablaTerceros).toString();
			apunte->setValue( "TABLATERCEROS", tablaterceros );
			dbRecord *tercero = DBAPP->createRecord(tablaterceros);
			if( tercero->read( pRecord->getValue(mTerceroIDField).toInt() ) ) 
				apunte->setValue( "TERCERO", tercero->toString(TOSTRING_CODE_AND_DESC) );
		} else {
			apunte->setValue( "TABLATERCEROS", mTablaTerceros );
			dbRecordRelation *rel_tercero = pRecord->findRelationByRelatedTable(mTablaTerceros);
			if( rel_tercero ) {
				dbRecord *tercero = rel_tercero->getRelatedRecord(-1);
				apunte->setValue( "TERCERO", tercero->toString(TOSTRING_CODE_AND_DESC) );
			}
		}
		apunte->setValue( "TERCERO_ID", pRecord->getValue(mTerceroIDField) );
	} else 
		apunte->setValue( "TERCERO", pRecord->getValue(mTerceroField) );
	if( mTablaConceptos == Xtring::null && mConceptoField == Xtring::null ) {
		apunte->setValue( "CONCEPTO", pRecord->toString(TOSTRING_CODE_AND_DESC) );
	} else if( mTablaConceptos != Xtring::null ) {
		if( mConceptosIsField ) {
			// No hay relación directa entre pRecord(Pago) y la tabla de conceptos.
			Xtring tablaconceptos = pRecord->getValue(mTablaConceptos).toString();
			apunte->setValue( "TABLACONCEPTOS", tablaconceptos );
			dbRecord *concepto = DBAPP->createRecord(tablaconceptos);
			if( concepto->read( pRecord->getValue(mConceptoIDField).toInt() ) ) 
				apunte->setValue( "CONCEPTO", concepto->toString(TOSTRING_CODE_AND_DESC) );
		} else {
			apunte->setValue( "TABLACONCEPTOS", mTablaConceptos );
			dbRecordRelation *rel_concepto = pRecord->findRelationByRelatedTable(mTablaConceptos);
			if( rel_concepto ) {
				dbRecord *concepto = rel_concepto->getRelatedRecord(-1);
				apunte->setValue( "CONCEPTO", concepto->toString(TOSTRING_CODE_AND_DESC) );
			}
		}
	} else 
		apunte->setValue( "CONCEPTO", pRecord->getValue(mConceptoField) );
	if( mNotasField != Xtring::null )
		apunte->setValue( "NOTAS", pRecord->getValue(mNotasField) );
	if( mProyectoIDField != Xtring::null )
		apunte->setValue( "PROYECTO_ID", pRecord->getValue(mProyectoIDField) );
	apunte->setValue("TABLADOCUMENTOS", pRecord->getTableName() );
	apunte->setValue("DOCUMENTO_ID", pRecord->getRecordID() );
	if( apunte->save(false) ) {
		pRecord->setValue(mApunteIDField, apunte->getRecordID() );
		if( pRecord->isModified() ) {
			pRecord->save( false );
		}
        DBAPP->showStickyOSD( pRecord->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
							  old_apunte->isRead() ? "Apunte regenerado en tesorería" : "Apunte generado en tesorería");
	}
	return apunte;
}


} // namespace tesoreria
} // namespace gong
