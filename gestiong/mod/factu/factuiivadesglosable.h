#ifndef _FACTU_IIVADESGLOSABLE_H
#define _FACTU_IIVADESGLOSABLE_H

#include <gongdbrecord.h>

namespace gong {
namespace factu {

class IIVADesglosable
{
public:
    IIVADesglosable( dbRecord *factura, dbRecordList *detalles )
        : pFactura(factura), pDetalles(detalles)
    {}
    Xtring calcDesgloseIVA( const Xtring &format = "%s\t(%s%%)", const Xtring &sep = "; " );
    bool hasRecargo() const;
protected:
    dbRecord *pFactura;
    dbRecordList *pDetalles;
};

} // namesace factu
} // namesace gong

#endif // _FACTU_IIVADESGLOSABLE_H
