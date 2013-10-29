#ifndef FACTUFRMREGENASIENTOS_H
#define FACTUFRMREGENASIENTOS_H

#include <dbappfrmcustom.h>

namespace gong {

class CheckBox;

namespace factu {

class FrmRegenAsientos: public FrmCustom
{
public:
    FrmRegenAsientos(QWidget *parent, const char *name, WidgetFlags f = 0);
    virtual void accept(); // from FrmCustom
protected:
    void regenAlbaranesCompra();
    void regenFacturasCompra();
    void regenAlbaranesVenta();
    void regenFacturasVenta();
    void regenPagos();
    void regenCobros();
    Xtring genSelect( const Xtring &tablename);
    virtual void validate_input( QWidget *, bool * ); // from FrmCustom

    DateRangeBox *pDateRange;
    CheckBox *chkRevisar, *chkAlbaranesCompra, *chkAlbaranesVenta,
             *chkFacturasCompra, *chkFacturasVenta, *chkCobros, *chkPagos;
};

} // namespace factu
} // namespace gong

#endif // FACTUFRMREGENASIENTOS_H
