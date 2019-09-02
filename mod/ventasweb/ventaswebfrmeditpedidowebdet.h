/*<<<<<COPYLEFT*/
/** @file ventaswebfrmeditpedidowebdet.h Fichero de edición de detalles de pedidos de venta
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<FRMEDITPEDIDOWEBDET_PREAMBLE*/
#ifndef _VENTASWEB_VENTASWEBFRMEDITPEDIDOWEBDET_H
#define _VENTASWEB_VENTASWEBFRMEDITPEDIDOWEBDET_H
/*>>>>>FRMEDITPEDIDOWEBDET_PREAMBLE*/
/*<<<<<FRMEDITPEDIDOWEBDET_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "ventaswebrecpedidowebdet.h"
/*>>>>>FRMEDITPEDIDOWEBDET_INCLUDES*/

namespace gong {
namespace ventasweb {

/*<<<<<FRMEDITPEDIDOWEBDET_CLASS*/
class FrmEditPedidoWebDet: public FrmEditRecDetail
{
	Q_OBJECT

public:
	FrmEditPedidoWebDet(FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecPedidoWebDet *getRecPedidoWebDet() const { return (RecPedidoWebDet *)getRecord(); }
/*>>>>>FRMEDITPEDIDOWEBDET_CLASS*/

    /*<<<<<FRMEDITPEDIDOWEBDET_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITPEDIDOWEBDET_VIRTUALS_FROM_FRMEDITREC*/

    /*<<<<<FRMEDITPEDIDOWEBDET_SCATTERS_AND_SLOTS*/
protected:
	void scatterArticulo();

private slots:
	void pushArticuloCodigo_clicked();

public:
	factu::RecArticulo* getRecArticulo() const
		{ return static_cast<RecPedidoWebDet*>(getRecord())->getRecArticulo(); }
/*>>>>>FRMEDITPEDIDOWEBDET_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITPEDIDOWEBDET_CONTROLS*/
protected:
	gong::EditBox *editCantidad;
	gong::SearchBox *searchArticuloCodigo;
	QPushButton *pushArticuloCodigo;
	gong::LineEdit *editArticuloCodigo;
	gong::LineEdit *editArticuloNombre;
	gong::EditBox *editPVP;
	gong::EditBox *editImporteConIVA;
/*>>>>>FRMEDITPEDIDOWEBDET_CONTROLS*/
};
/*<<<<<FRMEDITPEDIDOWEBDET_POSTAMBLE*/
} // namespace ventasweb
} // namespace gong
#endif // VENTASWEB_VENTASWEBFRMEDITPEDIDOWEBDET_H
/*>>>>>FRMEDITPEDIDOWEBDET_POSTAMBLE*/
