/*<<<<<FRMEDITCUENTATESORERIA_PREAMBLE*/
#ifndef _TESORERIA_TESORERIAFRMEDITCUENTATESORERIA_H
#define _TESORERIA_TESORERIAFRMEDITCUENTATESORERIA_H
/*>>>>>FRMEDITCUENTATESORERIA_PREAMBLE*/
/*<<<<<FRMEDITCUENTATESORERIA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "tesoreriareccuentatesoreria.h"
/*>>>>>FRMEDITCUENTATESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {


/*<<<<<COPYLEFT*/
/** @file tesoreriafrmeditcuentatesoreria.h Fichero de edición de cuentas de tesorería
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
/*<<<<<FRMEDITCUENTATESORERIA_CLASS*/
class FrmEditCuentaTesoreria: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditCuentaTesoreria(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecCuentaTesoreria *getRecCuentaTesoreria() const { return (RecCuentaTesoreria *)getRecord(); }
/*>>>>>FRMEDITCUENTATESORERIA_CLASS*/
/*<<<<<FRMEDITCUENTATESORERIA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITCUENTATESORERIA_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITCUENTATESORERIA_CONTROLS*/
protected:
	gong::EditBox *editDebe;
	gong::EditBox *editHaber;
	gong::EditBox *editSaldo;
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::EditBox *editSaldoInicial;
	gong::EditBox *editFechaSaldoInicial;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITCUENTATESORERIA_CONTROLS*/
	Variant mSaveSaldoInicial, mSaveSaldo;
    class FrmEditApunteTesoreria *pFrmApunteTesoreria;
	
};
/*<<<<<FRMEDITCUENTATESORERIA_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIAFRMEDITCUENTATESORERIA_H
/*>>>>>FRMEDITCUENTATESORERIA_POSTAMBLE*/

