/*<<<<<COPYLEFT*/
/** @file factufrmeditalbaranventadet.h Fichero de edición de detalles de albaranes de venta
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
/*<<<<<FRMEDITALBARANVENTADET_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITALBARANVENTADET_H
#define _FACTU_FACTUFRMEDITALBARANVENTADET_H
/*>>>>>FRMEDITALBARANVENTADET_PREAMBLE*/
/*<<<<<FRMEDITALBARANVENTADET_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "facturecalbaranventadet.h"
/*>>>>>FRMEDITALBARANVENTADET_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<FRMEDITALBARANVENTADET_CLASS*/
class FrmEditAlbaranVentaDet: public FrmEditRecDetail
{
	Q_OBJECT

public:
	FrmEditAlbaranVentaDet(FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecAlbaranVentaDet *getRecAlbaranVentaDet() const { return (RecAlbaranVentaDet *)getRecord(); }
/*>>>>>FRMEDITALBARANVENTADET_CLASS*/
    /*<<<<<FRMEDITALBARANVENTADET_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITALBARANVENTADET_VIRTUALS_FROM_FRMEDITREC*/
protected slots:
    void slotInsertTable_clicked();
    void slotActPrecioArticulo_clicked();
    /*<<<<<FRMEDITALBARANVENTADET_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoIVA();
	void scatterArticulo();

private slots:
	void pushTipoIVACodigo_clicked();
	void pushArticuloCodigo_clicked();

public:
	empresa::RecTipoIVA* getRecTipoIVA() const
		{ return static_cast<RecAlbaranVentaDet*>(getRecord())->getRecTipoIVA(); }
	RecArticulo* getRecArticulo() const
		{ return static_cast<RecAlbaranVentaDet*>(getRecord())->getRecArticulo(); }
/*>>>>>FRMEDITALBARANVENTADET_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITALBARANVENTADET_CONTROLS*/
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
/*>>>>>FRMEDITALBARANVENTADET_CONTROLS*/
    void updateStatus( bool callbehaviors );
    QPushButton *pushInsertTable, *pushActPrecioArticulo;
};
/*<<<<<FRMEDITALBARANVENTADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITALBARANVENTADET_H
/*>>>>>FRMEDITALBARANVENTADET_POSTAMBLE*/
