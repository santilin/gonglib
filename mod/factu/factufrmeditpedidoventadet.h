/*<<<<<COPYLEFT*/
/** @file factufrmeditpedidoventadet.h Fichero de edición de detalles de pedidos de venta
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
/*<<<<<FRMEDITPEDIDOVENTADET_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITPEDIDOVENTADET_H
#define _FACTU_FACTUFRMEDITPEDIDOVENTADET_H
/*>>>>>FRMEDITPEDIDOVENTADET_PREAMBLE*/
/*<<<<<FRMEDITPEDIDOVENTADET_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "facturecpedidoventadet.h"
/*>>>>>FRMEDITPEDIDOVENTADET_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<FRMEDITPEDIDOVENTADET_CLASS*/
class FrmEditPedidoVentaDet: public FrmEditRecDetail
{
	Q_OBJECT

public:
	FrmEditPedidoVentaDet(FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecPedidoVentaDet *getRecPedidoVentaDet() const { return (RecPedidoVentaDet *)getRecord(); }
/*>>>>>FRMEDITPEDIDOVENTADET_CLASS*/
    virtual void updateStatus( bool callbehaviors ); // from FrmEditRec

    /*<<<<<FRMEDITPEDIDOVENTADET_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITPEDIDOVENTADET_VIRTUALS_FROM_FRMEDITREC*/

protected slots:
    void slotInsertTable_clicked();
    void slotActPrecioArticulo_clicked();

    /*<<<<<FRMEDITPEDIDOVENTADET_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoIVA();
	void scatterArticulo();

private slots:
	void pushTipoIVACodigo_clicked();
	void pushArticuloCodigo_clicked();

public:
	empresa::RecTipoIVA* getRecTipoIVA() const
		{ return static_cast<RecPedidoVentaDet*>(getRecord())->getRecTipoIVA(); }
	RecArticulo* getRecArticulo() const
		{ return static_cast<RecPedidoVentaDet*>(getRecord())->getRecArticulo(); }
/*>>>>>FRMEDITPEDIDOVENTADET_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITPEDIDOVENTADET_CONTROLS*/
protected:
	gong::EditBox *editNLinea;
	gong::SearchBox *searchTipoIVACodigo;
	QPushButton *pushTipoIVACodigo;
	gong::LineEdit *editTipoIVACodigo;
	gong::LineEdit *editTipoIVANombre;
	gong::EditBox *editCantidad;
	gong::SearchBox *searchArticuloCodigo;
	QPushButton *pushArticuloCodigo;
	gong::LineEdit *editArticuloCodigo;
	gong::LineEdit *editArticuloNombre;
	gong::EditBox *editPVPSinIVA;
	gong::EditBox *editPVP;
	gong::EditBox *editDtoP100;
	gong::EditBox *editImporte;
	gong::EditBox *editImporteConIVA;
	gong::TextBox *editDescripcion;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITPEDIDOVENTADET_CONTROLS*/
private:
    QPushButton *pushInsertTable, *pushActPrecioArticulo;
};

/*<<<<<FRMEDITPEDIDOVENTADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITPEDIDOVENTADET_H
/*>>>>>FRMEDITPEDIDOVENTADET_POSTAMBLE*/

