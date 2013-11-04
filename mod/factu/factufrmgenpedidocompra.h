#ifndef FACTUFRMGENPEDIDOCOMPRA_H
#define FACTUFRMGENPEDIDOCOMPRA_H

#include <dbappfrmcustom.h>

namespace gong {

class CheckBox;

namespace factu {

class FrmGenPedidoCompra: public FrmCustom
{
public:
    FrmGenPedidoCompra(QWidget *parent, const char *name, WidgetFlags f = 0);
    virtual void accept(); // from FrmCustom
protected:
    void preparaDesdePedidosClientes();
    int genDesdePedidosClientes( bool agrupar_por_proveedor );
    void preparaDesdeStockArticulos();
    int genDesdeStockArticulos();
    void cambiaEstadoPedidosClientes();
    virtual void validate_input( QWidget *, bool * ); // from FrmCustom

    List<dbRecordID> mPedidoVentaIds, mPedidoVentaDetIds, mPedidosVentaACambiarEstado;
    CheckBox *chkAgruparPorProveedora;
    ComboBoxXtring *comboDesde;
    DateRangeBox *pDateRange;
    ComboBoxInt *comboEstadoPedidosClientes, *comboEstadoPedidosProveedores;
    CheckBox *chkRevisar;
    SearchBox *pSearchTipoDoc;
private:
    static XtringList mOpcionesDesde;
};

} // namespace factu
} // namespace gong

#endif // FACTUFRMGENPEDIDOCOMPRA_H
