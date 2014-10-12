#ifndef _GONG_FACTU_FRMPAGAR_H
#define _GONG_FACTU_FRMPAGAR_H

#include <dbappfrmcustom.h>

namespace gong {
namespace factu {

class FrmPagar: public FrmCustom
{
public:
    FrmPagar( bool has_contab, const Money &importe, Date fechapago,
              const Xtring &cuentapago, const Xtring &documentopago,
              QWidget *parent=0, const char *name=0, WidgetFlags f=0 );

	void accept();

	Date getFechaPago() {
        return editFechaPago->toDate();
    }
    Xtring getCuentaPago() {
        return searchCuentaPago->getEditCode()->toString();
    }
    Money getImporte() {
        return editImporte->toMoney();
    }
    Xtring getDocumentoPago() {
        return editDocumentoPago->toString();
    }
    static Xtring sLastCuentaPago, sLastDocumentoPago;
    static Date sLastFechaPago;
    virtual void validate_input( QWidget *search, bool *is_valid); // from FrmCustom
private:
    LineEdit *editFechaPago;
    SearchBox *searchCuentaPago;
    LineEdit *editImporte;
    LineEdit *editDocumentoPago;
    bool mHasContab;
};

} // namespace gong
} // namespace factu

#endif // _GONG_FACTU_FRMPAGAR_H

