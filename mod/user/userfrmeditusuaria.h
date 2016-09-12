/*<<<<<FRMEDITUSUARIA_PREAMBLE*/
#ifndef _USER_USERFRMEDITUSUARIA_H
#define _USER_USERFRMEDITUSUARIA_H
/*>>>>>FRMEDITUSUARIA_PREAMBLE*/
/*<<<<<COPYLEFT*/
/** @file userfrmeditusuaria.h Fichero de edición de usuarias
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
/*<<<<<FRMEDITUSUARIA_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "userrecusuaria.h"
/*>>>>>FRMEDITUSUARIA_INCLUDES*/

namespace gong {
namespace user {


/*<<<<<FRMEDITUSUARIA_CLASS*/
class FrmEditUsuaria: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditUsuaria(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecUsuaria *getRecUsuaria() const { return (RecUsuaria *)getRecord(); }
/*>>>>>FRMEDITUSUARIA_CLASS*/
/*<<<<<FRMEDITUSUARIA_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITUSUARIA_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITUSUARIA_CONTROLS*/
protected:
	gong::EditBox *editLogin;
	gong::EditBox *editPassword;
	gong::CheckBox *checkActiva;
	gong::TextBox *editNotas;
/*>>>>>FRMEDITUSUARIA_CONTROLS*/
};
/*<<<<<FRMEDITUSUARIA_POSTAMBLE*/
} // namespace user
} // namespace gong
#endif // USER_USERFRMEDITUSUARIA_H
/*>>>>>FRMEDITUSUARIA_POSTAMBLE*/

