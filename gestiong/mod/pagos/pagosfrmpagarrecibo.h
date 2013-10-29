#ifndef _GONG_PAGOS_FRMPAGARRECIBO_H
#define _GONG_PAGOS_FRMPAGARRECIBO_H

#include <dbappfrmcustom.h>

namespace gong {
namespace pagos {

class FrmPagarRecibo: public FrmCustom
{
public:
    FrmPagarRecibo( bool has_contab, const Money &importe, Date fechapago,
                    const Xtring &cuentapago, const Xtring &documentopago, const Xtring &moneda_codigo,
                    QWidget *parent=0, const char *name=0, WidgetFlags f=0 );
    void accept();
    Date getFechaPago() const {
        return editFechaPago->toDate();
    }
    Xtring getCuentaPago() const {
        return searchCuentaPago->getEditCode()->toString();
    }
    Money getImporte() const {
        return editImporte->toMoney();
    }
    Xtring getDocumentoPago() const {
        return editDocumentoPago->toString();
    }
    dbRecordID getRecMonedaID() const;
    virtual void validate_input( QWidget *search, bool *is_valid); // from FrmCustom
private:
    LineEdit *editFechaPago;
    SearchBox *searchCuentaPago, *searchMoneda;
    LineEdit *editImporte;
    LineEdit *editDocumentoPago;
    bool mHasContab;
};

} // namespace gong
} // namespace pagos

#endif // _GONG_PAGOS_FRMPAGARRECIBO_H

