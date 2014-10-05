#ifndef _FACTU_ITOTALIZABLERECORD_H
#define _FACTU_ITOTALIZABLERECORD_H

#include <gongdbrecordlist.h>

namespace gong {
namespace factu {

class RecArticulo;

class ITotalizableRecord
{
public:
    enum Tipo { compra = 0, venta };
    ITotalizableRecord( dbRecord *record, dbRecordList *detalles, Tipo tipo );
    void actTotales();
    bool addDescuentoRecargo( double dto, RecArticulo *articulo, int redondeo);
    void scatter( class FrmEditRec *frm );
    void gather( class FrmEditRec *frm );
protected:
    static XtringList sTotalFields;
    int getIvaDetallado() const;
    dbRecord *pRecord;
    dbRecordList *pDetalles;
    Tipo mTipo;
};

} // namesace factu
} // namesace gong

#endif // _FACTU_ITOTALIZABLERECORD_H
