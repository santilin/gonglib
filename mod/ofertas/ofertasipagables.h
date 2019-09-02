#ifndef _OFERTAS_IPAGABLES_H
#define _OFERTAS_IPAGABLES_H

#include <pagosipagablerecord.h>

namespace gong {
namespace ofertas {

class IPagableOferta: public pagos::IPagableRecord
{
public:
    IPagableOferta( dbRecord *factura, IPagableRecord::Tipo t )
        : IPagableRecord( factura, t, "TOTAL", (t == pagos ? "PAGOS" : "COBROS" ), "RESTO",
                          sOtherPagosFields, (t == pagos ? "PAGOOFERTA" : "COBROOFERTA") )
    {}
    virtual dbRecordID getProyectoID() const; // from pagos::IPagableRecord
    virtual dbRecord *getRecTercero() const; // from pagos::IPagableRecord
   
#ifdef HAVE_CONTABMODULE
    virtual Xtring getPrefijoContableTercero() const;
#endif
    static XtringList sOtherPagosFields;
};

} // namespace ofertas
} // namespace gong

#endif // _OFERTAS_IPAGABLES_H
