/*<<<<<COPYLEFT*/
/** @file ventaswebfrmeditpedidoweb.h Fichero de edición de pedidos web
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
/*<<<<<FRMEDITPEDIDOWEB_PREAMBLE*/
#ifndef _VENTASWEB_VENTASWEBFRMEDITPEDIDOWEB_H
#define _VENTASWEB_VENTASWEBFRMEDITPEDIDOWEB_H
/*>>>>>FRMEDITPEDIDOWEB_PREAMBLE*/
/*<<<<<FRMEDITPEDIDOWEB_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "ventaswebrecpedidoweb.h"
#include "ventaswebfrmeditpedidowebdet.h"
/*>>>>>FRMEDITPEDIDOWEB_INCLUDES*/

namespace gong {
namespace ventasweb {

/*<<<<<FRMEDITPEDIDOWEB_CLASS*/
class FrmEditPedidoWeb: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditPedidoWeb(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecPedidoWeb *getRecPedidoWeb() const { return (RecPedidoWeb *)getRecord(); }
/*>>>>>FRMEDITPEDIDOWEB_CLASS*/

    /*<<<<<FRMEDITPEDIDOWEB_CABECERA_DECL*/
protected:
	void updateFromDetails( class FrmEditRecDetail *detailfrm ); // from FrmEditRecMaster
	void actTotales();
	void numeraLineas();
	void genNumeroDocumento();
public:
/*>>>>>FRMEDITPEDIDOWEB_CABECERA_DECL*/
// {capel} Quitar función int getIvaDetallado()

    /*<<<<<FRMEDITPEDIDOWEB_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITPEDIDOWEB_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITPEDIDOWEB_SCATTERS_AND_SLOTS*/
protected:
	void scatterAgente();

private slots:
	void pushAgenteCodigo_clicked();

public:
	factu::RecAgente* getRecAgente() const
		{ return static_cast<RecPedidoWeb*>(getRecord())->getRecAgente(); }
/*>>>>>FRMEDITPEDIDOWEB_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITPEDIDOWEB_CONTROLS*/
protected:
	gong::EditBox *editContador;
	gong::ComboBoxInt *comboEstadoPedido;
	gong::EditBox *editFecha;
	gong::SearchBox *searchAgenteCodigo;
	QPushButton *pushAgenteCodigo;
	gong::LineEdit *editAgenteCodigo;
	gong::LineEdit *editAgenteRazonSocial;
	gong::EditBox *editReferencia;
	gong::EditBox *editNombre;
	gong::EditBox *editemail;
	gong::EditBox *editDireccion;
	gong::EditBox *editCP;
	gong::EditBox *editLocalidad;
	gong::EditBox *editProvincia;
	FrmEditPedidoWebDet *pFrmPedidoWebDet;
	gong::EditBox *editTotal;
	gong::TextBox *editTelefono;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITPEDIDOWEB_CONTROLS*/
};
/*<<<<<FRMEDITPEDIDOWEB_POSTAMBLE*/
} // namespace ventasweb
} // namespace gong
#endif // VENTASWEB_VENTASWEBFRMEDITPEDIDOWEB_H
/*>>>>>FRMEDITPEDIDOWEB_POSTAMBLE*/


