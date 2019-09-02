/*<<<<<FRMEDITCUENTAOFERTA_PREAMBLE*/
#ifndef _OFERTAS_OFERTASFRMEDITCUENTAOFERTA_H
#define _OFERTAS_OFERTASFRMEDITCUENTAOFERTA_H
/*>>>>>FRMEDITCUENTAOFERTA_PREAMBLE*/
/*<<<<<FRMEDITCUENTAOFERTA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "ofertasreccuentaoferta.h"
/*>>>>>FRMEDITCUENTAOFERTA_INCLUDES*/

namespace gong {
namespace ofertas {


/*<<<<<COPYLEFT*/
/** @file ofertasfrmeditcuentaoferta.h Fichero de edición de cuentas de tesorería
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
/*<<<<<FRMEDITCUENTAOFERTA_CLASS*/
class FrmEditCuentaOferta: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditCuentaOferta(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecCuentaOferta *getRecCuentaOferta() const { return (RecCuentaOferta *)getRecord(); }
/*>>>>>FRMEDITCUENTAOFERTA_CLASS*/
/*<<<<<FRMEDITCUENTAOFERTA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITCUENTAOFERTA_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITCUENTAOFERTA_CONTROLS*/
protected:
	gong::EditBox *editSaldo;
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::EditBox *editSaldoInicial;
	gong::EditBox *editFechaSaldoInicial;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITCUENTAOFERTA_CONTROLS*/
    Variant mSaveSaldoInicial, mSaveSaldo;
    class FrmEditApunteOferta *pFrmApunteOferta;
};
/*<<<<<FRMEDITCUENTAOFERTA_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASFRMEDITCUENTAOFERTA_H
/*>>>>>FRMEDITCUENTAOFERTA_POSTAMBLE*/

