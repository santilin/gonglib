#ifndef DEPOSIT_FRMESTADCOMPRAVENTA_H
#define DEPOSIT_FRMESTADCOMPRAVENTA_H

#include <factufrmestadcompraventa.h>

class QVBoxLayout;

namespace gong {
namespace tpv {

class FrmEstadCompraVenta: public factu::FrmEstadCompraVenta
{
public:
    FrmEstadCompraVenta(QWidget* parent = 0, WidgetFlags fl = 0 );

private:
	virtual Xtring getExtraSQL( ExtraSQLWhat what );
	SearchBox *pSearchSesionTPV;
};

} // namespace tpv
} // namespace gong

#endif // DEPOSIT_FRMESTADCOMPRAVENTA
