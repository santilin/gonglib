#ifndef SOCIASFRMGENERARRECIBOS_H
#define SOCIASFRMGENERARRECIBOS_H

#ifdef HAVE_PAGOSMODULE
#include <pagosrecformapago.h>
#include "pagosrecremesacobro.h"
#endif
#include <dbappfrmcustom.h>
#include "sociasrecproyecto.h"
#include "sociasrecpartidaproyecto.h"

class QVBoxLayout;

namespace gong {
namespace socias {

class FrmGenerarRecibos: public FrmCustom
{
    Q_OBJECT
public:
    FrmGenerarRecibos(QWidget* parent = 0, WidgetFlags fl = 0 );

private slots:
    void accept();
    void specialControlKeyPressed( QWidget *sender, char key );
    void pushPartidaCodigo_clicked();
    void scatterPartida();
    void pushRemesaCobroNumero_clicked();
    void scatterRemesaCobro();
    void pushCondiciones_clicked();
    void validate( QWidget *sender, bool *isvalid ); // from FrmCustom

private:
    QVBoxLayout *FrmGenerarRecibosLayout;
    SearchBox *searchPartidaCodigo;
    QPushButton *pushPartidaCodigo;
    LineEdit *editPartidaCodigo;
    LineEdit *editPartidaDescripcion;
    SearchBox *searchRemesaCobroNumero;
    QPushButton *pushRemesaCobroNumero;
    LineEdit *editRemesaCobroNumero;
    LineEdit *editRemesaCobroDescripcion;
    QPushButton *pushCondiciones;
    TextEdit *editCondiciones;
    LineEdit *editfechaini;
    LineEdit *editfechafin;
    LineEdit *editfechaemi;
    LineEdit *editfechavalor;
    LineEdit *editfechacargo;
    QCheckBox *checkinforme;
    QCheckBox *checkgenasiento;
    QCheckBox *checkSupervisar;
};

} // namespace socias
} // namespace gong

#endif
