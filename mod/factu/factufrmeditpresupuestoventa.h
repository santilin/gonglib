/*<<<<<COPYLEFT*/
/** @file factufrmeditpresupuestoventa.h Fichero de edición de presupuestos de venta
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
/*<<<<<FRMEDITPRESUPUESTOVENTA_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITPRESUPUESTOVENTA_H
#define _FACTU_FACTUFRMEDITPRESUPUESTOVENTA_H
/*>>>>>FRMEDITPRESUPUESTOVENTA_PREAMBLE*/
/*<<<<<FRMEDITPRESUPUESTOVENTA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "facturecpresupuestoventa.h"
#include "factufrmeditpresupuestoventadet.h"
/*>>>>>FRMEDITPRESUPUESTOVENTA_INCLUDES*/

namespace gong
{
namespace factu {


/*<<<<<FRMEDITPRESUPUESTOVENTA_CLASS*/
class FrmEditPresupuestoVenta: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditPresupuestoVenta(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecPresupuestoVenta *getRecPresupuestoVenta() const { return (RecPresupuestoVenta *)getRecord(); }
/*>>>>>FRMEDITPRESUPUESTOVENTA_CLASS*/

    /*<<<<<FRMEDITPRESUPUESTOVENTA_CABECERA_DECL*/
protected:
	void updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster
	void actTotales();
	void numeraLineas();
	void genNumeroDocumento();
public:
	int getIvaDetallado() const { return comboIVADetallado->getCurrentItemValue(); }
/*>>>>>FRMEDITPRESUPUESTOVENTA_CABECERA_DECL*/

    /*<<<<<FRMEDITPRESUPUESTOVENTA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITPRESUPUESTOVENTA_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITPRESUPUESTOVENTA_SCATTERS_AND_SLOTS*/
protected:
	void scatterTipoDoc();
	void scatterCliente();
	void scatterAgente();

private slots:
	void pushTipoDocCodigo_clicked();
	void pushClienteCodigo_clicked();
	void pushAgenteCodigo_clicked();

public:
	RecTipoDoc* getRecTipoDoc() const
		{ return static_cast<RecPresupuestoVenta*>(getRecord())->getRecTipoDoc(); }
	RecCliente* getRecCliente() const
		{ return static_cast<RecPresupuestoVenta*>(getRecord())->getRecCliente(); }
	RecAgente* getRecAgente() const
		{ return static_cast<RecPresupuestoVenta*>(getRecord())->getRecAgente(); }
/*>>>>>FRMEDITPRESUPUESTOVENTA_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITPRESUPUESTOVENTA_CONTROLS*/
protected:
	gong::EditBox *editFecha;
	gong::SearchBox *searchTipoDocCodigo;
	QPushButton *pushTipoDocCodigo;
	gong::LineEdit *editTipoDocCodigo;
	gong::LineEdit *editTipoDocNombre;
	gong::EditBox *editContador;
	gong::EditBox *editNumero;
	gong::SearchBox *searchClienteCodigo;
	QPushButton *pushClienteCodigo;
	gong::LineEdit *editClienteCodigo;
	gong::LineEdit *editClienteRazonSocial;
	gong::ComboBoxInt *comboIVADetallado;
	gong::SearchBox *searchAgenteCodigo;
	QPushButton *pushAgenteCodigo;
	gong::LineEdit *editAgenteCodigo;
	gong::LineEdit *editAgenteRazonSocial;
	FrmEditPresupuestoVentaDet *pFrmPresupuestoVentaDet;
	gong::EditBox *editSumaImportes;
	gong::EditBox *editDtoP100;
	gong::EditBox *editDescuento;
	gong::EditBox *editBaseImponible;
	gong::EditBox *editRecargoEquivalencia;
	gong::EditBox *editIVA;
	gong::EditBox *editTotal;
	gong::EditBox *editDesgloseIVA;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITPRESUPUESTOVENTA_CONTROLS*/
};
/*<<<<<FRMEDITPRESUPUESTOVENTA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITPRESUPUESTOVENTA_H
/*>>>>>FRMEDITPRESUPUESTOVENTA_POSTAMBLE*/

