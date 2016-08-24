/*<<<<<FRMEDITUSER_PREAMBLE*/
#ifndef _USER_USERFRMEDITUSUARIA_H
#define _USER_USERFRMEDITUSUARIA_H
/*>>>>>FRMEDITUSER_PREAMBLE*/
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
/*<<<<<FRMEDITUSER_INCLUDES*/
#include <gongfrmbase.h>
#include <dbappfrmeditrecmaster.h>
#include "userrecuser.h"
/*>>>>>FRMEDITUSER_INCLUDES*/

namespace gong {
namespace user {


/*<<<<<FRMEDITUSER_CLASS*/
class FrmEditUser: public FrmEditRecMaster
{
	Q_OBJECT

public:
	FrmEditUser(FrmEditRec *parentfrm, dbRecord *maestro, dbRecordDataModel *dm=0,
		EditMode editmode = DataTable::defaulteditmode, dbApplication::EditFlags flags = dbApplication::editNone,
		QWidget* parent = 0, const char* name = 0, WidgetFlags fl = 0 );
	RecUser *getRecUser() const { return (RecUser *)getRecord(); }
/*>>>>>FRMEDITUSER_CLASS*/
/*<<<<<FRMEDITUSER_VIRTUALS_FROM_FRMEDITREC*/
protected:
	virtual void scatterFields(); // From FrmEditRec
	virtual void gatherFields();
	virtual void validateFields(QWidget *sender, bool *isvalid, ValidResult *ir = 0);
/*>>>>>FRMEDITUSER_VIRTUALS_FROM_FRMEDITREC*/
/*<<<<<FRMEDITUSER_CONTROLS*/
protected:
	gong::EditBox *editName;
	gong::EditBox *editPassword;
/*>>>>>FRMEDITUSER_CONTROLS*/
};
/*<<<<<FRMEDITUSER_POSTAMBLE*/
} // namespace user
} // namespace gong
#endif // USER_USERFRMEDITUSUARIA_H
/*>>>>>FRMEDITUSER_POSTAMBLE*/

