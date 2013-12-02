#ifndef FACTUFRMESTADCOMPRAVENTA_H
#define FACTUFRMESTADCOMPRAVENTA_H

#include <dbappfrmcustom.h>
#include <QVBoxLayout>

class QVBoxLayout;

namespace gong {
namespace factu {

class FrmEstadCompraVenta: public FrmCustom
{
    Q_OBJECT
public:
    FrmEstadCompraVenta(QWidget* parent = 0, WidgetFlags fl = 0 );
protected slots:
    virtual void accept(); // from FrmBase
protected:
    virtual Xtring createRTK(const Xtring &_template, Xtring &from, Xtring &where,
                             const Xtring &compras_where, const Xtring &ventas_where, Xtring &titulo);
    enum ExtraSQLWhat {
        ComprasAlbaranesFrom, VentasAlbaranesFrom, ComprasFacturasFrom, VentasFacturasFrom,
        ComprasAlbaranesWhere, VentasAlbaranesWhere, ComprasFacturasWhere, VentasFacturasWhere,
        AnotherUnionAll
    };
    virtual Xtring getExtraSQL( ExtraSQLWhat what ) {
        return Xtring::null;
    }
    DateRangeBox *pDateRange;
    SearchBox *pSearchArticulo, *pSearchTipoDoc, *pSearchCliente, *pSearchProveedora;
    ComboBoxXtring *pComboAgruparPor, *pComboComprasOVentas;
    CheckBox *pCheckPreciosConIVA, *pCheckIncNotas, *pCheckMostrarAlbaranes, *pCheckCSV;
    CheckBox *pCheckResumido, *pCheckTodasLasEmpresas, *pCheckGranTotal;
    LineEdit *pEditTitulo;
};

#define ESTAD_SOLOCOMPRAS (pComboComprasOVentas->currentIndex() == 1)
#define ESTAD_SOLOVENTAS (pComboComprasOVentas->currentIndex() == 2)
#define ESTAD_COMPRAS (pComboComprasOVentas->currentIndex() == 0 || pComboComprasOVentas->currentIndex() == 1)
#define ESTAD_VENTAS (pComboComprasOVentas->currentIndex() == 0 || pComboComprasOVentas->currentIndex() == 2)
#define ESTAD_COMPRASYVENTAS (pComboComprasOVentas->currentIndex() == 0)

} // namespace factu
} // namespace gong
#endif // FACTUFRMESTADCOMPRAVENTA
