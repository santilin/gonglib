#ifndef GONG_CONTAB_FRMHERRAMIENTAS_H
#define GONG_CONTAB_FRMHERRAMIENTAS_H

#include "dbappfrmcustom.h"

namespace gong {
namespace contab {

class FrmHerramientas: public FrmCustom
{
public:
    FrmHerramientas( QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );

protected:
    virtual void validate_input( QWidget *sender, bool *isvalid ); // from FrmCustom
    void recalcularSaldos();
    void renumerarAsientos();
    QVBoxLayout *pFrmHerramientasLayout;
    QHBoxLayout *pMainLayout;
    QPushButton *pButtonRecalcularSaldos, *pButtonRenumerarAsientos;
};

} // namespace contab
} // namespace gong

#endif // GONG_CONTAB_FRMHERRAMIENTAS_H
