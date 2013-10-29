#ifndef FACTUFRMCOPIARDOCUMENTO_H
#define FACTUFRMCOPIARDOCUMENTO_H

#include <dbappfrmcustom.h>

namespace gong {

class CheckBox;

namespace factu {

class FrmCopiarDocumento: public FrmCustom
{
public:
    FrmCopiarDocumento(QWidget *parent, const char *name, WidgetFlags f = 0);
    virtual void accept(); // from FrmCustom
protected:
    virtual void validate_input( QWidget *sender, bool *isvalid );
    void copiaAlbaranCompra();
    void copiaAlbaranVenta();
    void copiaFacturaCompra();
    void copiaFacturaVenta();
    void copiaPedidoCompra();
    void copiaPedidoVenta();
    void copiaPago();
    void copiaCobro();

    LineEdit *pEditEjercicio;
    SearchBox *pSearchEmpresa;
    QPushButton *pButtonCopiaPedidoCompra,  *pButtonCopiaPedidoVenta;
    QPushButton *pButtonCopiaAlbaranCompra, *pButtonCopiaAlbaranVenta;
    QPushButton *pButtonCopiaFacturaCompra, *pButtonCopiaFacturaVenta;
};

} // namespace factu
} // namespace gong

#endif // FACTUFRMCOPIARDOCUMENTO
