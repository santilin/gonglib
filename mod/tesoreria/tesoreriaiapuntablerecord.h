#ifndef _TESORERIA_IAPUNTABLERECORD_H
#define _TESORERIA_IAPUNTABLERECORD_H

#include <gongdbrecord.h>
#include <gongdatatable.h>
#include "tesoreriareccuentatesoreria.h"
#include "tesoreriarecapuntetesoreria.h"

namespace gong {

class FrmEditRec;

namespace tesoreria {

class IApuntableRecord
{
public:
    IApuntableRecord( dbRecord *record, const Xtring &apunte_tesoreria_id_field, const Xtring &cuenta_tesoreria_id_field,
		const Xtring &fecha_field, const Xtring &importe_field, const Xtring &referencia_field, 
		const Xtring &tablaterceros, const Xtring &tercero_id_field, const Xtring &tercero_field,
		const Xtring &tablaconceptos, const Xtring &concepto_id_field, const Xtring &concepto_field,
		const Xtring &notas_field, const Xtring &proyecto_id_field);
    virtual RecApunteTesoreria *creaApunte( RecApunteTesoreria *old_apunte, bool supervisar );
    virtual dbRecordID regenApunte( bool supervisar );
    virtual RecApunteTesoreria *borraApunte( bool regenerando );
    virtual FrmEditRec *showApunte( DataTable::EditMode editmode = DataTable::selecting );
	RecCuentaTesoreria *getRecCuentaTesoreria() const;
	RecCuentaTesoreria *getRecCuentaPago() const { return getRecCuentaTesoreria(); }
protected:
    dbRecord *pRecord;
    Xtring mApunteIDField, mCuentaTesoreriaIDField, mFechaField, mImporteField, mReferenciaField,
		mTablaTerceros, mTerceroIDField, mTerceroField,
		mTablaConceptos, mConceptoIDField, mConceptoField,
		mNotasField, mProyectoIDField;
};

} // namesace tesoreria
} // namesace gong

#endif // _TESORERIA_IAPUNTABLERECORD_H
