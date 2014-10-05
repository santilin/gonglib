/*<<<<<COPYLEFT*/
/** @file pagosfrmeditformapago.h Formulario de edición de formas de pago
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
/*<<<<<FRMEDITFORMAPAGO_PREAMBLE*/
#ifndef _PAGOS_PAGOSFRMEDITFORMAPAGO_H
#define _PAGOS_PAGOSFRMEDITFORMAPAGO_H
/*>>>>>FRMEDITFORMAPAGO_PREAMBLE*/

#include "config.h"
/*<<<<<FRMEDITFORMAPAGO_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "pagosrecformapago.h"
/*>>>>>FRMEDITFORMAPAGO_INCLUDES*/
#ifdef HAVE_TESORERIAMODULE
#include <tesoreriareccuentatesoreria.h>
#endif

namespace gong {
namespace pagos {

/*<<<<<FRMEDITFORMAPAGO_CLASS*/
class FrmEditFormaPago: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditFormaPago(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecFormaPago *getRecFormaPago() const { return (RecFormaPago *)getRecord(); }
/*>>>>>FRMEDITFORMAPAGO_CLASS*/
    /*<<<<<FRMEDITFORMAPAGO_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
	void specialControlKeyPressed(QWidget *sender, char key);
/*>>>>>FRMEDITFORMAPAGO_VIRTUALS_FROM_FRMEDITREC*/
    void enableControlesPago();

    /*<<<<<FRMEDITFORMAPAGO_SCATTERS_AND_SLOTS*/
protected:
	void scatterCuentaTesoreria();

private slots:
	void pushCuentaTesoreriaCodigo_clicked();

public:
#ifdef HAVE_TESORERIAMODULE
	tesoreria::RecCuentaTesoreria* getRecCuentaTesoreria() const
		{ return static_cast<RecFormaPago*>(getRecord())->getRecCuentaTesoreria(); }
#endif
/*>>>>>FRMEDITFORMAPAGO_SCATTERS_AND_SLOTS*/

    /*<<<<<FRMEDITFORMAPAGO_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::ComboBoxInt *comboTipoFormaPago;
	gong::EditBox *editNumPlazos;
	gong::EditBox *editDiasEntrePlazos;
	gong::EditBox *editDiasPrimerPlazo;
	gong::EditBox *editDtoEnFactura;
#ifdef HAVE_CONTABMODULE
	gong::EditBox *editSubcuentaPago;
#endif
#ifdef HAVE_CONTABMODULE
	gong::EditBox *editSubcuentaCobro;
#endif
#ifdef HAVE_TESORERIAMODULE
	gong::SearchBox *searchCuentaTesoreriaCodigo;
	QPushButton *pushCuentaTesoreriaCodigo;
	gong::LineEdit *editCuentaTesoreriaCodigo;
	gong::LineEdit *editCuentaTesoreriaNombre;
#endif
/*>>>>>FRMEDITFORMAPAGO_CONTROLS*/
};

/*<<<<<FRMEDITFORMAPAGO_POSTAMBLE*/
} // namespace pagos
} // namespace gong
#endif // PAGOS_PAGOSFRMEDITFORMAPAGO_H
/*>>>>>FRMEDITFORMAPAGO_POSTAMBLE*/

