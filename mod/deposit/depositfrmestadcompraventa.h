#ifndef DEPOSIT_FRMESTADCOMPRAVENTA_H
#define DEPOSIT_FRMESTADCOMPRAVENTA_H

#include <factufrmestadcompraventa.h>

class QVBoxLayout;

namespace gong {
namespace deposit {

class FrmEstadCompraVenta: public factu::FrmEstadCompraVenta
{
public:
    FrmEstadCompraVenta(QWidget* parent = 0, WidgetFlags fl = 0 );

private:
	virtual Xtring createRTK(const Xtring &_template, Xtring &from, Xtring &where,
							 const Xtring &compras_where, const Xtring &ventas_where, Xtring &titulo);
	virtual Xtring getExtraSQL( ExtraSQLWhat what );
	SearchBox *pSearchTipoDocLiqDep;
	CheckBox *pCheckIncLiquidacionProveedora;
	GroupBox *pGroupLiquidacion;
	CheckBox *pCheckTodoLiquidado, *pCheckSoloLiquidado, *pCheckSoloNoLiquidado;
};

} // namespace deposit
} // namespace gong

#endif // DEPOSIT_FRMESTADCOMPRAVENTA
