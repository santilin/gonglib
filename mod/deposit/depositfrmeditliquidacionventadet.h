/*<<<<<COPYLEFT*/
/** @file depositfrmeditliquidacionventadet.h Fichero de edición de detalles de liquidaciones de venta
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
/*<<<<<FRMEDITLIQUIDACIONVENTADET_PREAMBLE*/
#ifndef _DEPOSIT_DEPOSITFRMEDITLIQUIDACIONVENTADET_H
#define _DEPOSIT_DEPOSITFRMEDITLIQUIDACIONVENTADET_H
/*>>>>>FRMEDITLIQUIDACIONVENTADET_PREAMBLE*/

/*<<<<<FRMEDITLIQUIDACIONVENTADET_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "depositrecliquidacionventadet.h"
/*>>>>>FRMEDITLIQUIDACIONVENTADET_INCLUDES*/

namespace gong {
namespace deposit {

/*<<<<<FRMEDITLIQUIDACIONVENTADET_CLASS*/
class FrmEditLiquidacionVentaDet: public FrmEditRecDetail
{
	Q_OBJECT

public:
	FrmEditLiquidacionVentaDet(FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecLiquidacionVentaDet *getRecLiquidacionVentaDet() const { return (RecLiquidacionVentaDet *)getRecord(); }
/*>>>>>FRMEDITLIQUIDACIONVENTADET_CLASS*/
	void updateStatus( bool callbehaviors );

/*<<<<<FRMEDITLIQUIDACIONVENTADET_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITLIQUIDACIONVENTADET_VIRTUALS_FROM_FRMEDITREC*/

	void slotActPrecioArticulo_clicked();
	void slotInsertTable_clicked();

/*<<<<<FRMEDITLIQUIDACIONVENTADET_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoIVA();
	void scatterArticulo();

private slots:
	void pushTipoIVACodigo_clicked();
	void pushArticuloCodigo_clicked();

public:
	empresa::RecTipoIVA* getRecTipoIVA() const
		{ return static_cast<RecLiquidacionVentaDet*>(getRecord())->getRecTipoIVA(); }
	factu::RecArticulo* getRecArticulo() const
		{ return static_cast<RecLiquidacionVentaDet*>(getRecord())->getRecArticulo(); }
/*>>>>>FRMEDITLIQUIDACIONVENTADET_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITLIQUIDACIONVENTADET_CONTROLS*/
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
	gong::EditBox *editImporteLiquidacion;
	gong::EditBox *editImporteLiquidacionIVA;
	gong::TextBox *editDescripcion;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITLIQUIDACIONVENTADET_CONTROLS*/
	QPushButton *pushActPrecioArticulo, *pushInsertTable;
};
/*<<<<<FRMEDITLIQUIDACIONVENTADET_POSTAMBLE*/
} // namespace deposit
} // namespace gong
#endif // DEPOSIT_DEPOSITFRMEDITLIQUIDACIONVENTADET_H
/*>>>>>FRMEDITLIQUIDACIONVENTADET_POSTAMBLE*/
