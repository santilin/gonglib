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
	pPushSegunProveedora = addButton(0, "Según proveedora");
}
void FrmGenCodArt::validate_input(QWidget* sender, bool* isvalid)
{
	if( sender == pPushSegunProveedora ) {
		mCodArt = pRecArticulo->genCodigoArticulo( pRecArticulo->getValue("PROVEEDORA.GENCODARTICULO").toInt(),
												   pRecArticulo->getValue("PROVEEDORA.FORMATCODARTICULO").toString() );
		accept();
 	}
}

void FrmGenCodArt::accept()
{
	if( mCodArt.isEmpty() ) {
		mCodArt = pRecArticulo->genCodigoArticulo( pComboOperacion->getCurrentItemValue(),
              pEditFormat->toString() );
	}
    FrmBase::accept();
}

} // namespace factu
} // namespace gong
