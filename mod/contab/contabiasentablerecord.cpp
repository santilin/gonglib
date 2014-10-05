#include <dbappdbapplication.h>
#include <dbappmainwindow.h>
#include <dbappfrmeditrec.h>
#include "contabiasentablerecord.h"

namespace gong {
namespace contab {

IAsentableRecord::IAsentableRecord(dbRecord *record, const Xtring &asiento_id_field)
    : pRecord( record ), mAsientoIDField( asiento_id_field )
{
}

RecCuenta *IAsentableRecord::getRecCuentaPago() const
{
    return static_cast<RecCuenta*>(pRecord->findRelatedRecord("CUENTAPAGO_ID"));
}

RecAsiento* IAsentableRecord::borraAsiento()
{
    RecAsiento *asiento = static_cast<RecAsiento *>(DBAPP->createRecord( "ASIENTO" ));
    if( pRecord->getValue( mAsientoIDField ).toInt() && asiento->read( pRecord->getValue( mAsientoIDField ).toInt() ) ) {
        asiento->remove();
    }
    pRecord->setValue( mAsientoIDField, 0 );
    return asiento;
}

dbRecordID IAsentableRecord::regenAsiento(bool supervisar)
{
    dbRecordID ret = 0;
    RecAsiento *old_asiento = borraAsiento();
    RecAsiento *asiento = creaAsiento( old_asiento, supervisar );
    if( asiento )
        ret = asiento->getRecordID();
    delete asiento;
    delete old_asiento;
    if( pRecord->isModified() ) {
        pRecord->save( false );
    }
    return ret;
}

FrmEditRec *IAsentableRecord::showAsiento( DataTable::EditMode editmode)
{
    FrmEditRec *frm = DBAPP->createEditForm(0, pRecord, 0, editmode);
    DBAPP->getMainWindow()->createClient( frm );
    return frm;
}


} // namespace contab
} // namespace gong
