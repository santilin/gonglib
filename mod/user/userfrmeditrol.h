/*<<<<<FRMEDITROL_PREAMBLE*/
#ifndef _USER_USERFRMEDITROL_H
#define _USER_USERFRMEDITROL_H
/*>>>>>FRMEDITROL_PREAMBLE*/
/*<<<<<FRMEDITROL_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "userrecrol.h"
/*>>>>>FRMEDITROL_INCLUDES*/

namespace gong {
namespace user {

/*<<<<<COPYLEFT*/
/** @file userfrmeditrol.h Fichero de edición de roles del módulo user
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
/*<<<<<FRMEDITROL_CLASS*/
class FrmEditRol: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditRol(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecRol *getRecRol() const { return (RecRol *)getRecord(); }
/*>>>>>FRMEDITROL_CLASS*/
/*<<<<<FRMEDITROL_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITROL_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITROL_CONTROLS*/
protected:
	gong::EditBox *editNombre;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITROL_CONTROLS*/
};
/*<<<<<FRMEDITROL_POSTAMBLE*/
} // namespace user
} // namespace gong
#endif // USER_USERFRMEDITROL_H
/*>>>>>FRMEDITROL_POSTAMBLE*/

