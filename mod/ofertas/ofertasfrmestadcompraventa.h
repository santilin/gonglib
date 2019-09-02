#ifndef OFERTAS_FRMESTADCOMPRAVENTA_H
#define OFERTAS_FRMESTADCOMPRAVENTA_H

#include <factufrmestadcompraventa.h>

namespace gong {
namespace ofertas {

class FrmEstadCompraVenta: public factu::FrmEstadCompraVenta
{
public:
    FrmEstadCompraVenta(QWidget* parent = 0, WidgetFlags fl = 0 );

private:
	virtual Xtring createRTK(const Xtring &_template, Xtring &from, Xtring &where,
							 const Xtring &compras_where, const Xtring &ventas_where, Xtring &titulo);
    virtual Xtring getExtraSQL( ExtraSQLWhat what, const Xtring &signo_ventas, bool usa_prov_articulo, 
		const Xtring &compras_where, const Xtring &ventas_where,
		const List<dbRecordID> &proveedora_ids, const List<dbRecordID> &cliente_ids,
		const List<dbRecordID> &agente_ids, const List<dbRecordID> &formapago_ids,
		const List<dbRecordID> &articulo_ids, const List<dbRecordID> &familia_ids,
		const List<dbRecordID> &tipodoc_ids);
	CheckBox *pCheckIncOfertas;
};

} // namespace ofertas
} // namespace gong

#endif // OFERTAS_FRMESTADCOMPRAVENTA
