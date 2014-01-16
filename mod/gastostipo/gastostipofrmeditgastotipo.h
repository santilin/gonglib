/*<<<<<COPYLEFT*/
/** @file gastosfrmeditgastodef.h Fichero de edición de definiciones de gastos e ingresos
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
/*<<<<<FRMEDITGASTODEF_PREAMBLE*/
#ifndef _GASTOS_GASTOSFRMEDITGASTODEF_H
#define _GASTOS_GASTOSFRMEDITGASTODEF_H
/*>>>>>FRMEDITGASTODEF_PREAMBLE*/
/*<<<<<FRMEDITGASTODEF_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "gastosrecgastodef.h"
/*>>>>>FRMEDITGASTODEF_INCLUDES*/

namespace gong {
namespace gastos {

/*<<<<<FRMEDITGASTODEF_CLASS*/
class FrmEditGastoDef: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditGastoDef(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecGastoDef *getRecGastoDef() const { return (RecGastoDef *)getRecord(); }
/*>>>>>FRMEDITGASTODEF_CLASS*/
/*<<<<<FRMEDITGASTODEF_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITGASTODEF_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITGASTODEF_CONTROLS*/
protected:
	gong::ComboBoxInt *comboTipoGasto;
	gong::ComboBoxInt *comboCategoriaGasto;
	gong::ComboBoxInt *comboPedirImporte;
	gong::EditBox *editImporte;
	gong::ComboBoxInt *comboPedirDescripcion;
	gong::EditBox *editDescripcion;
	gong::ComboBoxInt *comboPedirNotas;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITGASTODEF_CONTROLS*/
};
/*<<<<<FRMEDITGASTODEF_POSTAMBLE*/
} // namespace gastos
} // namespace gong
#endif // GASTOS_GASTOSFRMEDITGASTODEF_H
/*>>>>>FRMEDITGASTODEF_POSTAMBLE*/

