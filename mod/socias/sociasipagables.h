#ifndef _SOCIAS_IPAGABLES_H
#define _SOCIAS_IPAGABLES_H

#include <pagosipagablerecord.h>

namespace gong {
namespace socias {

class IPagableMiembro: public pagos::IPagableRecord
{
public:
    IPagableMiembro( dbRecord *miembro )
        : IPagableRecord( miembro, IPagableRecord::cobros,
                          "IMPORTETOTAL", "PAGOSTOTAL", "RESTOTOTAL", XtringList() )
    {}
    virtual dbRecordID getProyectoID() const;
    virtual dbRecord *getRecTercero() const;
#ifdef HAVE_CONTABMODULE
    virtual Xtring getPrefijoContableTercero() const;
#endif
};

} // namespace socias
} // namespace gong

#endif // _SOCIAS_IPAGABLES_H
