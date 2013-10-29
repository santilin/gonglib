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
private:
    class RecArticulo *pRecArticulo;
    ComboBox<int> *pComboOperacion;
    LineEdit *pEditFormat;
    Xtring mCodArt;
};

} // namespace factu
} // namespace gong

#endif // FACTU_FRMGENCODART_H
