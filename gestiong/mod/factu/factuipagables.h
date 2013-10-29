#ifndef _FACTU_IPAGABLES_H
#define _FACTU_IPAGABLES_H

#include <pagosipagablerecord.h>

namespace gong {
namespace factu {

class IPagableAlbaran: public pagos::IPagableRecord
{
public:
    IPagableAlbaran( dbRecord *factura, IPagableRecord::Tipo t )
        : IPagableRecord( factura, t, "TOTAL", (t == pagos ? "PAGOS" : "COBROS" ), "RESTO",
                          sOtherPagosFields )
    {}
    virtual dbRecordID getProyectoID() const;
    virtual dbRecord *getRecTercero() const;
#ifdef HAVE_CONTABMODULE
    virtual Xtring getPrefijoContableTercero() const;
#endif
    static XtringList sOtherPagosFields;
};


class IPagableFactura: public pagos::IPagableRecord
{
public:
    IPagableFactura( dbRecord *factura, IPagableRecord::Tipo t )
        : IPagableRecord( factura, t, "TOTAL", (t == pagos ? "PAGOS" : "COBROS" ), "RESTO",
                          sOtherPagosFields )
    {}
    virtual dbRecordID getProyectoID() const;
    virtual dbRecord *getRecTercero() const;
#ifdef HAVE_CONTABMODULE
    virtual Xtring getPrefijoContableTercero() const;
#endif
    static XtringList sOtherPagosFields;
};

} // namesace factu
} // namesace gong

#endif // _FACTU_IPAGABLES_H
