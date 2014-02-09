#ifndef FACTUFRMMOD303_H
#define FACTUFRMMOD303_H

#include <dbappfrmcustom.h>
#include <QVBoxLayout>

class QVBoxLayout;

namespace gong {
namespace factu {

class FrmMod303: public FrmCustom
{
public:
    FrmMod303(QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    virtual void accept(); // from FrmBase

private:
    ComboBoxXtring *pComboOpciones, *pComboTrimestre;
    CheckBox *pCheckSepararIvas;
    SearchBox *pSearchTipoDoc;
};

} // namespace factu
} // namespace gong

#endif // FACTUFRMMOD303_H
