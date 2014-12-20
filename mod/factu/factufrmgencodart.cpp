#include <gonggettext.h>
#include <empresamodule.h>
#include "facturecarticulo.h"
#include "factufrmgencodart.h"

namespace gong {
namespace factu {

FrmGenCodArt::FrmGenCodArt ( RecArticulo *articulo,
                             QWidget* parent, const char* name, WidgetFlags fl )
    : FrmCustom(parent, name, fl)
{
    pRecArticulo = articulo;
    setTitle( _("Generar código del artículo") );
    pComboOperacion = addComboBoxInt(true, 0, _("Modalidad"),
                                     RecArticulo::sModalidades, IdentityIntList );
    pEditFormat = addInput(0, _("Formato"), "", "STRING");
}

void FrmGenCodArt::accept()
{
    mCodArt = pRecArticulo->genCodigoArticulo( pComboOperacion->getCurrentItemValue(),
              pEditFormat->toString() );
    close();
}

} // namespace factu
} // namespace gong
