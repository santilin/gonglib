#include "factumodule.h"
#include "facturecalbarancompra.h"
#include "facturecalbaranventa.h"
#include "facturecfacturacompra.h"
#include "facturecfacturaventa.h"
#include "factuipagables.h"

namespace gong {
namespace factu {

XtringList IPagableAlbaran::sOtherPagosFields = XtringList() << "ENTREGA";
XtringList IPagableFactura::sOtherPagosFields = XtringList() << "ENTREGA" << "ENTREGAALBARANES";

dbRecordID IPagableAlbaran::getProyectoID() const
{
    return pFactura->getValue( "PROYECTO_ID" ).toInt();
}


dbRecord *IPagableAlbaran::getRecTercero() const
{
    if( mTipo == pagos ) {
        return dynamic_cast<RecAlbaranCompra *>(pFactura)->getRecProveedora();
    } else {
        return dynamic_cast<RecAlbaranVenta *>(pFactura)->getRecCliente();
    }
}

#ifdef HAVE_CONTABMODULE
Xtring IPagableAlbaran::getPrefijoContableTercero() const
{
    if( mTipo == pagos ) {
        return ModuleInstance->getContabModule()->getModuleSetting( "CUENTA_PROVEEDORA" ).toString();
    } else {
        return ModuleInstance->getContabModule()->getModuleSetting( "CUENTA_CLIENTE" ).toString();
    }
}

Xtring IPagableFactura::getPrefijoContableTercero() const
{
    if( mTipo == pagos ) {
        return ModuleInstance->getContabModule()->getModuleSetting( "CUENTA_PROVEEDORA" ).toString();
    } else {
        return ModuleInstance->getContabModule()->getModuleSetting( "CUENTA_CLIENTE" ).toString();
    }
}
#endif

dbRecordID IPagableFactura::getProyectoID() const
{
    return pFactura->getValue( "PROYECTO_ID" ).toInt();
}


dbRecord *IPagableFactura::getRecTercero() const
{
    if( mTipo == pagos ) {
        return dynamic_cast<RecFacturaCompra *>(pFactura)->getRecProveedora();
    } else {
        return dynamic_cast<RecFacturaVenta *>(pFactura)->getRecCliente();
    }
}


} // namespace factu
} // namespace gong
