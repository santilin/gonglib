#ifndef FACTU_FRMGENCODART_H
#define FACTU_FRMGENCODART_H

#include <dbappfrmcustom.h>

namespace gong {
namespace factu {

class FrmGenCodArt: public FrmCustom
{
public:
    FrmGenCodArt(class RecArticulo *articulo,
                 QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    Xtring getCodArt() const {
        return mCodArt;
    }
    virtual void accept(); // from FrmBase
    virtual void validate_input( QWidget *sender, bool *isvalid ); // from FrmCustom	
private:
    class RecArticulo *pRecArticulo;
    ComboBoxInt *pComboOperacion;
    LineEdit *pEditFormat;
	PushButton *pPushSegunProveedora;
    Xtring mCodArt;
};

} // namespace factu
} // namespace gong

#endif // FACTU_FRMGENCODART_H
