#ifndef FACTUFRMBALANCECLIPRO_H
#define FACTUFRMBALANCECLIPRO_H

#include <QVBoxLayout>
#include <dbappfrmreportconds.h>

class QVBoxLayout;

namespace gong {
namespace factu {

class FrmBalanceCliPro: public FrmReportConds
{
public:
    FrmBalanceCliPro(QWidget* parent = 0, WidgetFlags fl = 0 );
    virtual void accept(); // from FrmBase

private:
    Xtring createRTK(const Xtring &_template, Xtring &from, Xtring &where, Xtring &titulo);

    DateRangeBox *pDateRange;
    SearchBox *pSearchTipoDoc, *pSearchCliente, *pSearchProveedora;
    ComboBoxXtring *pComboAgruparPor, *pComboCliOPro;
    CheckBox *pCheckIncCobros, *pCheckTodasLasEmpresas, *pCheckGranTotal, *pCheckIncNotas;
};

} // namespace factu
} // namespace gong

#endif // FACTUFRMBALANCECLIPRO_H
