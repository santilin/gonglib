/*<<<<<COPYLEFT*/
/** @file factufrmeditpedidoventa.h Fichero de edición de pedidos de venta
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
/*<<<<<FRMEDITPEDIDOVENTA_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITPEDIDOVENTA_H
#define _FACTU_FACTUFRMEDITPEDIDOVENTA_H
/*>>>>>FRMEDITPEDIDOVENTA_PREAMBLE*/
/*<<<<<FRMEDITPEDIDOVENTA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "facturecpedidoventa.h"
#include "factufrmeditpedidoventadet.h"
/*>>>>>FRMEDITPEDIDOVENTA_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<FRMEDITPEDIDOVENTA_CLASS*/
class FrmEditPedidoVenta: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditPedidoVenta(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecPedidoVenta *getRecPedidoVenta() const { return (RecPedidoVenta *)getRecord(); }
/*>>>>>FRMEDITPEDIDOVENTA_CLASS*/

    /*<<<<<FRMEDITPEDIDOVENTA_CABECERA_DECL*/
protected:
	void updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster
	void actTotales();
	void numeraLineas();
	void genNumeroDocumento();
public:
	int getIvaDetallado() const { return comboIVADetallado->getCurrentItemValue(); }
/*>>>>>FRMEDITPEDIDOVENTA_CABECERA_DECL*/

    /*<<<<<FRMEDITPEDIDOVENTA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITPEDIDOVENTA_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITPEDIDOVENTA_SCATTERS_AND_SLOTS*/
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
		{ return static_cast<RecPedidoVenta*>(getRecord())->getRecTipoDoc(); }
	RecCliente* getRecCliente() const
		{ return static_cast<RecPedidoVenta*>(getRecord())->getRecCliente(); }
	RecAgente* getRecAgente() const
		{ return static_cast<RecPedidoVenta*>(getRecord())->getRecAgente(); }
/*>>>>>FRMEDITPEDIDOVENTA_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITPEDIDOVENTA_CONTROLS*/
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
	gong::ComboBoxInt *comboEstadoPedido;
	FrmEditPedidoVentaDet *pFrmPedidoVentaDet;
	gong::EditBox *editReferencia;
	gong::EditBox *editSumaImportes;
	gong::EditBox *editDtoP100;
	gong::EditBox *editDescuento;
	gong::EditBox *editBaseImponible;
	gong::EditBox *editRecargoEquivalencia;
	gong::EditBox *editIVA;
	gong::EditBox *editTotal;
	gong::EditBox *editDesgloseIVA;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITPEDIDOVENTA_CONTROLS*/
};
/*<<<<<FRMEDITPEDIDOVENTA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITPEDIDOVENTA_H
/*>>>>>FRMEDITPEDIDOVENTA_POSTAMBLE*/


