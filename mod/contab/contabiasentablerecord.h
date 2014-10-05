#ifndef _CONTAB_IASENTABLERECORD_H
#define _CONTAB_IASENTABLERECORD_H

#include <gongdbrecord.h>
#include <gongdatatable.h>
#include "contabreccuenta.h"
#include "contabrecasiento.h"

namespace gong {

class FrmEditRec;

namespace contab {

class IAsentableRecord
{
public:
    IAsentableRecord( dbRecord *record, const Xtring &asiento_id_field );
    virtual dbRecord *getRecTercero() const = 0;
    virtual RecAsiento *creaAsiento( RecAsiento *old_asiento, bool supervisar ) = 0;
    virtual RecCuenta *getRecCuentaPago() const;
    virtual dbRecordID regenAsiento( bool supervisar );
    virtual RecAsiento *borraAsiento();
    virtual FrmEditRec *showAsiento( DataTable::EditMode editmode = DataTable::selecting );
protected:
    dbRecord *pRecord;
    Xtring mAsientoIDField;
};

} // namesace contab
} // namesace gong

#endif // _CONTAB_IASENTABLERECORD_H
