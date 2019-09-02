#ifndef _OFERTAS_OFERTASFRMEDITPAGOOFERTA_H
#define _OFERTAS_OFERTASFRMEDITPAGOOFERTA_H

#include <pagosfrmeditpago.h>
#include "ofertasrecpagooferta.h"

namespace gong {
namespace ofertas {

class FrmEditPagoOferta: public pagos::FrmEditPago
{
public:
    FrmEditPagoOferta(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
                      EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
                      QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
};

/*<<<<<FRMEDITPAGOOFERTA_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASFRMEDITPAGOOFERTA_H
/*>>>>>FRMEDITPAGOOFERTA_POSTAMBLE*/

