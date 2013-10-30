#ifndef FACTUFRMBALANCECLIPRO_H
#define FACTUFRMBALANCECLIPRO_H

#include <dbappfrmcustom.h>
#include <QVBoxLayout>

class QVBoxLayout;

namespace gong {
namespace factu {

class FrmBalanceCliPro: public FrmCustom
{
public:
    FrmBalanceCliPro(QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
    virtual void accept(); // from FrmBase

private:
    Xtring createRTK(Xtring &from, Xtring &where, Xtring &titulo);

    DateRangeBox *pDateRange;
    SearchBox *pSearchTipoDoc, *pSearchCliente, *pSearchProveedora;
    ComboBox<Xtring> *pComboAgruparPor, *pComboCliOPro;
    CheckBox *pCheckIncCobros, *pCheckCSV;
    CheckBox *pCheckResumido, *pCheckTodasLasEmpresas, *pCheckGranTotal, *pCheckIncNotas;
    LineEdit *pEditTitulo;
};

} // namespace factu
} // namespace gong

#endif // FACTUFRMBALANCECLIPRO_H
