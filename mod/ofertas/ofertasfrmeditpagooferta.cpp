#include "ofertasfrmeditpagooferta.h"

namespace gong {
namespace ofertas {

/*<<<<<FRMEDITPAGOOFERTA_CONSTRUCTOR*/
FrmEditPagoOferta::FrmEditPagoOferta(FrmEditRec *parentfrm, dbRecord *master, dbRecordDataModel *dm,
                                     EditMode editmode, dbApplication::EditFlags editflags,
                                     QWidget *parent, const char* name, WidgetFlags fl )
    : FrmEditPago( parentfrm, master, dm, editmode, editflags, parent, "FrmEditPagoOferta", fl )
{
}

/*<<<<<FRMEDITPAGOOFERTA_FIN*/
} // namespace ofertas
} // namespace gong
/*>>>>>FRMEDITPAGOOFERTA_FIN*/
