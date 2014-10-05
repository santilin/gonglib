#include <dbappdbapplication.h>
#include <dbappmainwindow.h>
#include <dbappfrmeditrec.h>
#include "tesoreriaiapuntablerecord.h"

namespace gong {
namespace tesoreria {

IApuntableRecord::IApuntableRecord(dbRecord* record, const Xtring& apunte_id_field, const Xtring& cuenta_pago_id_field, 
	const Xtring& fecha_field, const Xtring& importe_field, const Xtring& referencia_field, 
	const Xtring& tablaterceros, const Xtring& tercero_id_field, const Xtring& tercero_field, 
	const Xtring& tablaconceptos, const Xtring& concepto_id_field, const Xtring& concepto_field, 
	const Xtring& notas_field, const Xtring& proyecto_id_field)
	: pRecord(record), mApunteIDField(apunte_id_field), mCuentaPagoIDField(cuenta_pago_id_field), mFechaField(fecha_field), 
		mImporteField(importe_field), mReferenciaField(referencia_field), mTablaTerceros(tablaterceros), 
		mTerceroIDField(tercero_id_field), mTerceroField(tercero_field), mTablaConceptos(tablaconceptos),
		mConceptoIDField(concepto_id_field), mConceptoField(concepto_field), mNotasField(notas_field),
		mProyectoIDField(proyecto_id_field)
{
}

/*
RecCuentaTesoreria *IApuntableRecord::getRecCuentaPago() const
{
    return static_cast<RecCuentaTesoreria*>(pRecord->findRelatedRecord("CUENTAPAGO_ID"));
}
*/

RecApunteTesoreria* IApuntableRecord::borraApunte()
{
    RecApunteTesoreria *apunte = static_cast<RecApunteTesoreria *>(DBAPP->createRecord( "APUNTETESORERIA" ));
    if( pRecord->getValue( mApunteIDField ).toInt() && apunte->read( pRecord->getValue( mApunteIDField ).toInt() ) ) {
        apunte->remove();
    }
    pRecord->setValue( mApunteIDField, 0 );
    return apunte;
}

dbRecordID IApuntableRecord::regenApunte(bool supervisar)
{
    dbRecordID ret = 0;
    RecApunteTesoreria *old_apunte = borraApunte();
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
	/// TODO: Buscar el apunte anterior para modificarlo 
	if( pRecord->getValue(mImporteField).toDouble() == 0.0 )
		return 0;
	RecApunteTesoreria *apunte = static_cast<RecApunteTesoreria *>(DBAPP->createRecord("APUNTETESORERIA"));
	apunte->setValue( "EMPRESA_ID", pRecord->getValue("EMPRESA_ID") );
	apunte->setValue( "EJERCICIO", pRecord->getValue("EJERCICIO") );
	apunte->setValue( "FECHA", pRecord->getValue(mFechaField) );
	apunte->setValue( "IMPORTE", pRecord->getValue(mImporteField) );
	apunte->setValue( "REFERENCIA", pRecord->getValue(mReferenciaField) );
	if( mTablaTerceros == Xtring::null && mTerceroField == Xtring::null ) {
	} else if( mTablaTerceros != Xtring::null ) {
		apunte->setValue( "TABLATERCEROS", mTablaTerceros );
		apunte->setValue( "TERCERO_ID", pRecord->getValue(mTerceroIDField) );
	} else 
		apunte->setValue( "TERCERO", pRecord->getValue(mTerceroField) );
	if( mTablaConceptos == Xtring::null && mConceptoField == Xtring::null ) {
		apunte->setValue( "CONCEPTO", pRecord->toString(TOSTRING_CODE_AND_DESC) );
	} else if( mTablaConceptos != Xtring::null ) {
		apunte->setValue( "TABLACONCEPTOS", mTablaConceptos );
		apunte->setValue( "CONCEPTO_ID", pRecord->getValue(mConceptoIDField) );
	} else 
		apunte->setValue( "CONCEPTO", pRecord->getValue(mConceptoField) );
	if( mNotasField != Xtring::null )
		apunte->setValue( "NOTAS", pRecord->getValue(mNotasField) );
	if( mProyectoIDField != Xtring::null )
		apunte->setValue( "PROYECTO_ID", pRecord->getValue(mProyectoIDField) );
	if( apunte->save(false) ) {
		pRecord->setValue(mApunteIDField, apunte->getRecordID() );
		if( pRecord->isModified() ) {
			pRecord->save( false );
		}
        DBAPP->showStickyOSD( pRecord->toString( TOSTRING_CODE_AND_DESC_WITH_TABLENAME ),
							  "Apunte generado en tesorería");
	}
	return apunte;
}


} // namespace contab
} // namespace gong
