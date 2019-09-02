/*<<<<<COPYLEFT*/
/** @file depositfrmeditliquidacionventa.h Fichero de edición de liquidaciones de ventas
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
/*<<<<<FRMEDITLIQUIDACIONVENTA_PREAMBLE*/
#ifndef _DEPOSIT_DEPOSITFRMEDITLIQUIDACIONVENTA_H
#define _DEPOSIT_DEPOSITFRMEDITLIQUIDACIONVENTA_H
/*>>>>>FRMEDITLIQUIDACIONVENTA_PREAMBLE*/
/*<<<<<FRMEDITLIQUIDACIONVENTA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "depositrecliquidacionventa.h"
#include "depositfrmeditliquidacionventadet.h"
/*>>>>>FRMEDITLIQUIDACIONVENTA_INCLUDES*/

namespace gong {
namespace deposit {

	/*<<<<<FRMEDITLIQUIDACIONVENTA_CLASS*/
class FrmEditLiquidacionVenta: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditLiquidacionVenta(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecLiquidacionVenta *getRecLiquidacionVenta() const { return (RecLiquidacionVenta *)getRecord(); }
/*>>>>>FRMEDITLIQUIDACIONVENTA_CLASS*/

/*<<<<<FRMEDITLIQUIDACIONVENTA_CABECERA_DECL*/
protected:
	void updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster
	void actTotales();
	void numeraLineas();
	void genNumeroDocumento();
public:
	int getIvaDetallado() const { return comboIVADetallado->getCurrentItemValue(); }
/*>>>>>FRMEDITLIQUIDACIONVENTA_CABECERA_DECL*/

/*<<<<<FRMEDITLIQUIDACIONVENTA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITLIQUIDACIONVENTA_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITLIQUIDACIONVENTA_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoDoc();
	void scatterCliente();
	void scatterAgente();

private slots:
	void pushTipoDocCodigo_clicked();
	void pushClienteCodigo_clicked();
	void pushAgenteCodigo_clicked();

public:
	factu::RecTipoDoc* getRecTipoDoc() const
		{ return static_cast<RecLiquidacionVenta*>(getRecord())->getRecTipoDoc(); }
	factu::RecCliente* getRecCliente() const
		{ return static_cast<RecLiquidacionVenta*>(getRecord())->getRecCliente(); }
	factu::RecAgente* getRecAgente() const
		{ return static_cast<RecLiquidacionVenta*>(getRecord())->getRecAgente(); }
/*>>>>>FRMEDITLIQUIDACIONVENTA_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITLIQUIDACIONVENTA_CONTROLS*/
protected:
	gong::EditBox *editFecha;
	gong::SearchBox *searchTipoDocCodigo;
	QPushButton *pushTipoDocCodigo;
	gong::LineEdit *editTipoDocCodigo;
	gong::LineEdit *editTipoDocNombre;
	gong::EditBox *editNumero;
	gong::ComboBoxInt *comboIVADetallado;
	gong::EditBox *editContador;
	gong::SearchBox *searchClienteCodigo;
	QPushButton *pushClienteCodigo;
	gong::LineEdit *editClienteCodigo;
	gong::LineEdit *editClienteRazonSocial;
	gong::SearchBox *searchAgenteCodigo;
	QPushButton *pushAgenteCodigo;
	gong::LineEdit *editAgenteCodigo;
	gong::LineEdit *editAgenteRazonSocial;
	FrmEditLiquidacionVentaDet *pFrmLiquidacionVentaDet;
	gong::EditBox *editSumaImportes;
	gong::EditBox *editDtoP100;
	gong::EditBox *editDescuento;
	gong::EditBox *editBaseImponible;
	gong::EditBox *editIVA;
	gong::EditBox *editRecargoEquivalencia;
	gong::EditBox *editTotal;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITLIQUIDACIONVENTA_CONTROLS*/
};

/*<<<<<FRMEDITLIQUIDACIONVENTA_POSTAMBLE*/
} // namespace deposit
} // namespace gong
#endif // DEPOSIT_DEPOSITFRMEDITLIQUIDACIONVENTA_H
/*>>>>>FRMEDITLIQUIDACIONVENTA_POSTAMBLE*/

