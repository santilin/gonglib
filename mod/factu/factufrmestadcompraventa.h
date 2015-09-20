#ifndef FACTUFRMESTADCOMPRAVENTA_H
#define FACTUFRMESTADCOMPRAVENTA_H

#include <dbappfrmreportconds.h>

namespace gong {
namespace factu {

class FrmEstadCompraVenta: public FrmReportConds
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
    virtual Xtring getExtraSQL( ExtraSQLWhat what, const Xtring &signo_ventas, bool usa_prov_articulo, 
		const Xtring &ventas_where, const Xtring &compras_where,
		const List<dbRecordID> &proveedora_ids, const List<dbRecordID> &cliente_ids,
		const List<dbRecordID> &agente_ids, const List<dbRecordID> &formapago_ids,
		const List<dbRecordID> &articulo_ids, const List<dbRecordID> &familia_ids,
		const List<dbRecordID> &tipodoc_ids)
	{
        return Xtring::null;
    }
    DateRangeBox *pDateRange;
    SearchBox *pSearchArticulo, *pSearchTipoDoc, *pSearchCliente, *pSearchProveedora,
              *pSearchFamilia, *pSearchAgente, *pSearchFormaPago;
    ComboBoxXtring *pComboAgruparPor, *pComboComprasOVentas, *pComboAlbaranesFacturas;
    CheckBox *pCheckPreciosConIVA, *pCheckIncNotas, *pCheckTodasLasEmpresas, *pCheckGranTotal;
};

#define ESTAD_SOLOCOMPRAS (pComboComprasOVentas->currentIndex() == 1)
#define ESTAD_SOLOVENTAS (pComboComprasOVentas->currentIndex() == 2)
#define ESTAD_COMPRAS (pComboComprasOVentas->currentIndex() == 0 || pComboComprasOVentas->currentIndex() == 1)
#define ESTAD_VENTAS (pComboComprasOVentas->currentIndex() == 0 || pComboComprasOVentas->currentIndex() == 2)
#define ESTAD_COMPRASYVENTAS (pComboComprasOVentas->currentIndex() == 0)
#define ESTAD_ALBARANESYFACTURAS (pComboAlbaranesFacturas->currentIndex() == 0 )
#define ESTAD_SOLOFACTURAS (pComboAlbaranesFacturas->currentIndex() == 1 )
#define ESTAD_SOLOALBARANES (pComboAlbaranesFacturas->currentIndex() == 2 )
} // namespace factu
} // namespace gong
#endif // FACTUFRMESTADCOMPRAVENTA
