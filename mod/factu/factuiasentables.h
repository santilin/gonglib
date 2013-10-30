#ifndef _FACTU_IASENTABLES_H
#define _FACTU_IASENTABLES_H

#include "config.h"
#ifdef HAVE_CONTABMODULE
#include <pagosrecformapago.h>
#include <contabiasentablerecord.h>

namespace gong {
namespace factu {

class IAsentableFactura: public contab::IAsentableRecord
{
public:
    enum Tipo { facturacompra, facturaventa, albarancompra, albaranventa };
    IAsentableFactura( dbRecord *record, Tipo tipo, const Xtring &asiento_id_field = "ASIENTO_ID" );
    virtual dbRecord *getRecTercero() const;
    virtual contab::RecAsiento *creaAsiento( contab::RecAsiento *old_asiento, bool supervisar );
protected:
    contab::RecAsiento *creaAsientoSimple( contab::RecAsiento *old_asiento, bool supervisar );
    contab::RecAsiento *creaAsientoCompletoCompra( dbRecordList *detalles,
            contab::RecAsiento *old_asiento, bool supervisar );
    contab::RecAsiento *creaAsientoCompletoVenta( dbRecordList *detalles,
            contab::RecAsiento *old_asiento, bool supervisar );
    pagos::RecFormaPago *getRecFormaPago() const;
    Tipo mTipo;
};

} // namespace factu
} // namespace gong

#endif // HAVE_CONTABMODULE

#endif // _FACTU_IASENTABLES_H
