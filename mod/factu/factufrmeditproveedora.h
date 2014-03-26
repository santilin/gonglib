/*<<<<<COPYLEFT*/
/** @file factufrmeditproveedora.h Fichero de edición de proveedoras
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
/*<<<<<FRMEDITPROVEEDORA_PREAMBLE*/
#ifndef _FACTU_FACTUFRMEDITPROVEEDORA_H
#define _FACTU_FACTUFRMEDITPROVEEDORA_H
/*>>>>>FRMEDITPROVEEDORA_PREAMBLE*/

#include "config.h"
/*<<<<<FRMEDITPROVEEDORA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "facturecproveedora.h"
/*>>>>>FRMEDITPROVEEDORA_INCLUDES*/
#include "facturecagente.h"

namespace gong {
namespace contactos {
class FrmEditContactoBehavior;
}
namespace factu {

/*<<<<<FRMEDITPROVEEDORA_CLASS*/
class FrmEditProveedora: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditProveedora(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecProveedora *getRecProveedora() const { return (RecProveedora *)getRecord(); }
/*>>>>>FRMEDITPROVEEDORA_CLASS*/

    /*<<<<<FRMEDITPROVEEDORA_SCATTERS_AND_SLOTS*/
protected:
	void scatterFormaPago();
	void scatterAgente();

private slots:
	void pushFormaPagoCodigo_clicked();
	void pushAgenteCodigo_clicked();

public:
	pagos::RecFormaPago* getRecFormaPago() const
		{ return static_cast<RecProveedora*>(getRecord())->getRecFormaPago(); }
	RecAgente* getRecAgente() const
		{ return static_cast<RecProveedora*>(getRecord())->getRecAgente(); }
/*>>>>>FRMEDITPROVEEDORA_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITPROVEEDORA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITPROVEEDORA_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITPROVEEDORA_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editRazonSocial;
	gong::SearchBox *searchFormaPagoCodigo;
	QPushButton *pushFormaPagoCodigo;
	gong::LineEdit *editFormaPagoCodigo;
	gong::LineEdit *editFormaPagoNombre;
	gong::EditBox *editNombreAlt;
	gong::SearchBox *searchAgenteCodigo;
	QPushButton *pushAgenteCodigo;
	gong::LineEdit *editAgenteCodigo;
	gong::LineEdit *editAgenteRazonSocial;
	gong::ComboBoxInt *comboIVADetallado;
	gong::EditBox *editEntidadBanco;
	gong::EditBox *editCuentaBanco;
#ifdef HAVE_CONTABMODULE
	gong::EditBox *editSubcuenta;
#endif
	gong::ComboBoxInt *comboGenCodArticulo;
	gong::EditBox *editFormatCodArticulo;
	gong::CheckBox *checkUsaReferencias;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITPROVEEDORA_CONTROLS*/
    class contactos::FrmEditContactoBehavior *pEditContactoBehavior;
    class QTabWidget *pFrameContactos;
};

/*<<<<<FRMEDITPROVEEDORA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUFRMEDITPROVEEDORA_H
/*>>>>>FRMEDITPROVEEDORA_POSTAMBLE*/

