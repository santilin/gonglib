#include "sociasmodule.h"
#include "sociasipagables.h"
#include "sociasrecmiembro.h"

namespace gong {
namespace socias {

dbRecordID IPagableMiembro::getProyectoID() const
{
    return pFactura->getValue( "PROYECTO_ID" ).toInt();
}

dbRecord *IPagableMiembro::getRecTercero() const
{
#ifdef HAVE_FACTUMODULE
    if( ModuleInstance->getFactuModule() )
        return dynamic_cast<RecMiembro *>(pFactura)->getRecCliente();
    else
#endif
        return dynamic_cast<RecMiembro *>(pFactura)->getRecContacto();
}

#ifdef HAVE_CONTABMODULE
Xtring IPagableMiembro::getPrefijoContableTercero() const
{
    return ModuleInstance->getContabModule()->getModuleSetting( "CUENTA_MIEMBRO" ).toString();
}
#endif

} // namespace socias
} // namespace gong
