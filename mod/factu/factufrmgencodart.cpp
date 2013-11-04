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
    pComboOperacion = addComboBoxInt(0, _("Modalidad"), RecArticulo::sModalidades,
                                  IdentityIntList );
    pEditFormat = addInput(0, _("Formato"), "", "STRING");
}

void FrmGenCodArt::accept()
{
    mCodArt = pRecArticulo->genCodigoArticulo( pComboOperacion->getCurrentItemValue(),
              pEditFormat->toString(), pRecArticulo->getValue( "NOMBRE" ).toString(),
              pRecArticulo->getValue( "FAMILIA.CODIGO" ).toInt(),
              pRecArticulo->getValue( "FAMILIA.NOMBRE" ).toString(),
              pRecArticulo->getValue( "PROVEEDORA.CODIGO" ).toInt(),
              pRecArticulo->getValue( "PROVEEDORA.RAZONSOCIAL" ).toString() );
    close();
}

} // namespace factu
} // namespace gong
