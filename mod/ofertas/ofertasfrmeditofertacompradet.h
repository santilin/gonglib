/*<<<<<COPYLEFT*/
/** @file ofertasfrmeditofertacompradet.h Fichero de edición de detalles de albaranes de compra
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
/*<<<<<FRMEDITOFERTACOMPRADET_PREAMBLE*/
#ifndef _OFERTAS_OFERTASFRMEDITOFERTACOMPRADET_H
#define _OFERTAS_OFERTASFRMEDITOFERTACOMPRADET_H
/*>>>>>FRMEDITOFERTACOMPRADET_PREAMBLE*/
/*<<<<<FRMEDITOFERTACOMPRADET_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "ofertasrecofertacompradet.h"
/*>>>>>FRMEDITOFERTACOMPRADET_INCLUDES*/

namespace gong {
namespace ofertas {


/*<<<<<FRMEDITOFERTACOMPRADET_CLASS*/
class FrmEditOfertaCompraDet: public FrmEditRecDetail
{
	Q_OBJECT

public:
	FrmEditOfertaCompraDet(FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecOfertaCompraDet *getRecOfertaCompraDet() const { return (RecOfertaCompraDet *)getRecord(); }
/*>>>>>FRMEDITOFERTACOMPRADET_CLASS*/
    /*<<<<<FRMEDITOFERTACOMPRADET_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITOFERTACOMPRADET_VIRTUALS_FROM_FRMEDITREC*/

protected slots:
    void slotInsertTable_clicked();
    void slotActPrecioArticulo_clicked();

    /*<<<<<FRMEDITOFERTACOMPRADET_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoIVA();
	void scatterArticulo();

private slots:
	void pushTipoIVACodigo_clicked();
	void pushArticuloCodigo_clicked();

public:
	empresa::RecTipoIVA* getRecTipoIVA() const
		{ return static_cast<RecOfertaCompraDet*>(getRecord())->getRecTipoIVA(); }
	factu::RecArticulo* getRecArticulo() const
		{ return static_cast<RecOfertaCompraDet*>(getRecord())->getRecArticulo(); }
/*>>>>>FRMEDITOFERTACOMPRADET_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITOFERTACOMPRADET_CONTROLS*/
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
	gong::EditBox *editCosteSinIVA;
	gong::EditBox *editCoste;
	gong::EditBox *editDtoP100;
	gong::EditBox *editImporte;
	gong::EditBox *editImporteConIVA;
	gong::TextBox *editDescripcion;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITOFERTACOMPRADET_CONTROLS*/
    virtual void updateStatus( bool callbehaviors );
    QPushButton *pushInsertTable, *pushActPrecioArticulo;
};
/*<<<<<FRMEDITOFERTACOMPRADET_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASFRMEDITOFERTACOMPRADET_H
/*>>>>>FRMEDITOFERTACOMPRADET_POSTAMBLE*/

