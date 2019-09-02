#include "ofertasmodule.h"
#include "ofertasrecofertacompra.h"
#include "ofertasrecofertaventa.h"
#include "ofertasipagables.h"

namespace gong {
namespace ofertas {

XtringList IPagableOferta::sOtherPagosFields = XtringList() << "ENTREGA";

dbRecordID IPagableOferta::getProyectoID() const
{
    return 0;
}

dbRecord *IPagableOferta::getRecTercero() const
{
    if( mTipo == pagos ) {
        return dynamic_cast<RecOfertaCompra *>(pFactura)->getRecProveedora();
    } else {
        return dynamic_cast<RecOfertaVenta *>(pFactura)->getRecCliente();
    }
}


#ifdef HAVE_CONTABMODULE
Xtring IPagableOferta::getPrefijoContableTercero() const
{
    return Xtring::null;
}
#endif

} // namespace ofertas
} // namespace gong
