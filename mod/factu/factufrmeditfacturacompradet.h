/*<<<<<COPYLEFT*/
/** @file factufrmeditfacturacompradet.h Fichero de edición de detalles de factura de compra
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
/*<<<<<FRMEDITFACTURACOMPRADET_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITFACTURACOMPRADET_H
#define _FACTU_FACTUFRMEDITFACTURACOMPRADET_H
/*>>>>>FRMEDITFACTURACOMPRADET_PREAMBLE*/
/*<<<<<FRMEDITFACTURACOMPRADET_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecdetail.h>
#include "facturecfacturacompradet.h"
/*>>>>>FRMEDITFACTURACOMPRADET_INCLUDES*/

namespace gong
{
namespace factu {


/*<<<<<FRMEDITFACTURACOMPRADET_CLASS*/
class FrmEditFacturaCompraDet: public FrmEditRecDetail
{
	Q_OBJECT

public:
	FrmEditFacturaCompraDet(FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *detail, const Xtring &dettablename, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecFacturaCompraDet *getRecFacturaCompraDet() const { return (RecFacturaCompraDet *)getRecord(); }
/*>>>>>FRMEDITFACTURACOMPRADET_CLASS*/
protected:
    virtual void updateStatus( bool callbehaviors ); // from FrmEditRec
    virtual bool canBeginEdit( EditMode newmode); // from FrmEditRecDetail

    /*<<<<<FRMEDITFACTURACOMPRADET_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITFACTURACOMPRADET_VIRTUALS_FROM_FRMEDITREC*/

protected slots:
    void slotAddAlbaran_clicked();
    void slotInsertTable_clicked();
    void slotActPrecioArticulo_clicked();

    /*<<<<<FRMEDITFACTURACOMPRADET_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoIVA();
	void scatterArticulo();

private slots:
	void pushTipoIVACodigo_clicked();
	void pushArticuloCodigo_clicked();

public:
	empresa::RecTipoIVA* getRecTipoIVA() const
		{ return static_cast<RecFacturaCompraDet*>(getRecord())->getRecTipoIVA(); }
	RecArticulo* getRecArticulo() const
		{ return static_cast<RecFacturaCompraDet*>(getRecord())->getRecArticulo(); }
/*>>>>>FRMEDITFACTURACOMPRADET_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITFACTURACOMPRADET_CONTROLS*/
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
/*>>>>>FRMEDITFACTURACOMPRADET_CONTROLS*/
    QPushButton *pushInsertTable, *pushAddAlbaran, *pushActPrecioArticulo;
};

/*<<<<<FRMEDITFACTURACOMPRADET_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITFACTURACOMPRADET_H
/*>>>>>FRMEDITFACTURACOMPRADET_POSTAMBLE*/
