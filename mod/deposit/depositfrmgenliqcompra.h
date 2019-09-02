#ifndef DEPOSIT_FRMGENDEPOSITOCOMPRA_H
#define DEPOSIT_FRMGENDEPOSITOCOMPRA_H

#include <dbappfrmcustom.h>

namespace gong {
namespace deposit {

class FrmGenLiqCompra: public FrmCustom
{
public:
	FrmGenLiqCompra(QWidget *parent = 0, WidgetFlags f = 0);
	virtual void accept(); // from FrmBase
protected:
	class RecLiquidacionCompra *genLiquidacionCompra( dbRecordID proveedora_id,
				dbRecordID tipodocliq_id, List<dbRecordID> &avdet_ids,
				const Xtring &cond = Xtring::null );
	DateRangeBox *pDateRange;
	SearchBox *pSearchTipoDocCond, *pSearchTipoDocLiquidacion, *pSearchProveedora;
	CheckBox *pCheckReliquidar, *pCheckMarcarVentasLiquidadas;
};

} // namespace deposit
} // namespace gong

#endif // DEPOSIT_FRMGENDEPOSITOCOMPRA_H
