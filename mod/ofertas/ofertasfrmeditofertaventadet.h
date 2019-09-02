/*<<<<<COPYLEFT*/
/** @file ofertasfrmeditofertaventadet.h Fichero de edición de detalles de albaranes de venta
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
/*<<<<<FRMEDITOFERTAVENTADET_PREAMBLE*/
#ifndef _OFERTAS_OFERTASFRMEDITOFERTAVENTADET_H
#define _OFERTAS_OFERTASFRMEDITOFERTAVENTADET_H
/*>>>>>FRMEDITOFERTAVENTADET_PREAMBLE*/
/*<<<<<FRMEDITOFERTAVENTADET_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "ofertasrecofertaventadet.h"
/*>>>>>FRMEDITOFERTAVENTADET_INCLUDES*/

namespace gong {
namespace ofertas {

/*<<<<<FRMEDITOFERTAVENTADET_CLASS*/
class FrmEditOfertaVentaDet: public FrmEditRecDetail
{
	Q_OBJECT

public:
	FrmEditOfertaVentaDet(FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecOfertaVentaDet *getRecOfertaVentaDet() const { return (RecOfertaVentaDet *)getRecord(); }
/*>>>>>FRMEDITOFERTAVENTADET_CLASS*/
    /*<<<<<FRMEDITOFERTAVENTADET_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITOFERTAVENTADET_VIRTUALS_FROM_FRMEDITREC*/

protected slots:
    void slotInsertTable_clicked();
    void slotActPrecioArticulo_clicked();

    /*<<<<<FRMEDITOFERTAVENTADET_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoIVA();
	void scatterArticulo();

private slots:
	void pushTipoIVACodigo_clicked();
	void pushArticuloCodigo_clicked();

public:
	empresa::RecTipoIVA* getRecTipoIVA() const
		{ return static_cast<RecOfertaVentaDet*>(getRecord())->getRecTipoIVA(); }
	factu::RecArticulo* getRecArticulo() const
		{ return static_cast<RecOfertaVentaDet*>(getRecord())->getRecArticulo(); }
/*>>>>>FRMEDITOFERTAVENTADET_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITOFERTAVENTADET_CONTROLS*/
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
/*>>>>>FRMEDITOFERTAVENTADET_CONTROLS*/
    void updateStatus( bool callbehaviors );
    QPushButton *pushInsertTable, *pushActPrecioArticulo;
};
/*<<<<<FRMEDITOFERTAVENTADET_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASFRMEDITOFERTAVENTADET_H
/*>>>>>FRMEDITOFERTAVENTADET_POSTAMBLE*/


