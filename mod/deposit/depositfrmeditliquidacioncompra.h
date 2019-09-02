/*<<<<<COPYLEFT*/
/** @file depositfrmeditliquidacioncompra.h Fichero de edición de liquidaciones de compras
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
/*<<<<<FRMEDITLIQUIDACIONCOMPRA_PREAMBLE*/
#ifndef _DEPOSIT_DEPOSITFRMEDITLIQUIDACIONCOMPRA_H
#define _DEPOSIT_DEPOSITFRMEDITLIQUIDACIONCOMPRA_H
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_PREAMBLE*/
/*<<<<<FRMEDITLIQUIDACIONCOMPRA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "depositrecliquidacioncompra.h"
#include "depositfrmeditliquidacioncompradet.h"
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_INCLUDES*/

namespace gong {
namespace deposit {

	/*<<<<<FRMEDITLIQUIDACIONCOMPRA_CLASS*/
class FrmEditLiquidacionCompra: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditLiquidacionCompra(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecLiquidacionCompra *getRecLiquidacionCompra() const { return (RecLiquidacionCompra *)getRecord(); }
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_CLASS*/

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_CABECERA_DECL*/
protected:
	void updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster
	void actTotales();
	void numeraLineas();
	void genNumeroDocumento();
public:
	int getIvaDetallado() const { return comboIVADetallado->getCurrentItemValue(); }
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_CABECERA_DECL*/

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_VIRTUALS_FROM_FRMEDITREC*/

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoDoc();
	void scatterProveedora();

private slots:
	void pushTipoDocCodigo_clicked();
	void pushProveedoraCodigo_clicked();

public:
	factu::RecTipoDoc* getRecTipoDoc() const
		{ return static_cast<RecLiquidacionCompra*>(getRecord())->getRecTipoDoc(); }
	factu::RecProveedora* getRecProveedora() const
		{ return static_cast<RecLiquidacionCompra*>(getRecord())->getRecProveedora(); }
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_SCATTERS_AND_SLOTS*/

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_CONTROLS*/
protected:
	gong::EditBox *editFecha;
	gong::SearchBox *searchTipoDocCodigo;
	QPushButton *pushTipoDocCodigo;
	gong::LineEdit *editTipoDocCodigo;
	gong::LineEdit *editTipoDocNombre;
	gong::EditBox *editContador;
	gong::EditBox *editNumero;
	gong::SearchBox *searchProveedoraCodigo;
	QPushButton *pushProveedoraCodigo;
	gong::LineEdit *editProveedoraCodigo;
	gong::LineEdit *editProveedoraRazonSocial;
	gong::ComboBoxInt *comboIVADetallado;
	FrmEditLiquidacionCompraDet *pFrmLiquidacionCompraDet;
	gong::EditBox *editSumaImportes;
	gong::EditBox *editDtoP100;
	gong::EditBox *editDescuento;
	gong::EditBox *editBaseImponible;
	gong::EditBox *editIVA;
	gong::EditBox *editRecargoEquivalencia;
	gong::EditBox *editTotal;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_CONTROLS*/
};

/*<<<<<FRMEDITLIQUIDACIONCOMPRA_POSTAMBLE*/
} // namespace deposit
} // namespace gong
#endif // DEPOSIT_DEPOSITFRMEDITLIQUIDACIONCOMPRA_H
/*>>>>>FRMEDITLIQUIDACIONCOMPRA_POSTAMBLE*/

