#ifndef DEPOSIT_FRMFACTLIQVENTA_H
#define DEPOSIT_FRMFACTLIQVENTA_H

#include <dbappfrmcustom.h>

namespace gong {

namespace factu { class RecFacturaVenta; }

namespace deposit {

class FrmFactLiqVenta: public FrmCustom
{
public:
	FrmFactLiqVenta(QWidget *parent = 0, WidgetFlags f = 0);
	virtual void accept(); // from FrmBase
protected:
	void genFacturaVenta( factu::RecFacturaVenta *facturaventa,
						  dbRecordID cli_id, const Xtring &where = Xtring::null );
	DateRangeBox *pDateRange;
	dbRecordID mClienteID;
	SearchBox *pSearchTipoDocCond, *pSearchCliente;
};

} // namespace deposit
} // namespace gong

#endif // DEPOSIT_FRMFACTLIQVENTA_H
