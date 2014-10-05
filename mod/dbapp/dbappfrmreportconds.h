#ifndef _DBAPP_FRMREPORTCONDS_H
#define _DBAPP_FRMREPORTCONDS_H

#include <dbappfrmcustom.h>

class QVBoxLayout;

namespace gong {

class FrmReportConds: public FrmCustom
{
public:
    FrmReportConds(const Xtring &titulo,
                   QWidget* parent = 0, WidgetFlags fl = 0 );
protected:
    Xtring readRTK( const Xtring &_template );

    LineEdit *pEditTitulo;
    ComboBoxInt *pComboOutputs;
    CheckBox *pCheckResumido, *pCheckApaisado;
};

} // namespace gong
#endif // _DBAPP_FRMREPORTCONDS_H
