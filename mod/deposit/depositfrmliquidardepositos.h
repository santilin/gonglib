#ifndef DEPOSIT_FRMLIQUIDARDEPOSITOS_H
#define DEPOSIT_FRMLIQUIDARDEPOSITOS_H

#include <dbappfrmcustom.h>

namespace gong {
namespace deposit {

class RecLiquidacionVenta;

class FrmLiquidarDepositos: public FrmCustom
{
public:
	enum Tipo { compras, ventas };
	FrmLiquidarDepositos(Tipo tipo, QWidget *parent = 0, WidgetFlags f = 0);
	virtual void accept(); // from FrmBase
protected:
	RecLiquidacionVenta *genLiquidacionVenta( dbRecordID cliente_id,	const Xtring &cond );
	Tipo mTipo;
	DateRangeBox *pDateRange;
	SearchBox *pSearchTipoDocCond, *pSearchTercero;
};

} // namespace deposit
} // namespace gong

#endif // DEPOSIT_FRMLIQUIDARDEPOSITOS_H
