/*<<<<<COPYLEFT*/
/** @file empresafrmedittipoiva.h Fichero de edición de tipos de IVA
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
/*<<<<<FRMEDITTIPOIVA_PREAMBLE*/
#ifndef _EMPRESA_EMPRESAFRMEDITTIPOIVA_H
#define _EMPRESA_EMPRESAFRMEDITTIPOIVA_H
/*>>>>>FRMEDITTIPOIVA_PREAMBLE*/

/*<<<<<FRMEDITTIPOIVA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "empresarectipoiva.h"
/*>>>>>FRMEDITTIPOIVA_INCLUDES*/
#include "empresarectipoiva.h"

namespace gong
{
namespace empresa {

/*<<<<<FRMEDITTIPOIVA_CLASS*/
class FrmEditTipoIVA: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditTipoIVA(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecTipoIVA *getRecTipoIVA() const { return (RecTipoIVA *)getRecord(); }
/*>>>>>FRMEDITTIPOIVA_CLASS*/
    /*<<<<<FRMEDITTIPOIVA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITTIPOIVA_VIRTUALS_FROM_FRMEDITREC*/
    /*<<<<<FRMEDITTIPOIVA_CONTROLS*/
protected:
	gong::EditBox *editCodigo;
	gong::EditBox *editNombre;
	gong::EditBox *editIVA;
	gong::EditBox *editRecargo;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITTIPOIVA_CONTROLS*/
};

/*<<<<<FRMEDITTIPOIVA_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESAFRMEDITTIPOIVA_H
/*>>>>>FRMEDITTIPOIVA_POSTAMBLE*/

