/*<<<<<USERMODULE_PREAMBLE*/
#ifndef _USER_USERMODULE_H
#define _USER_USERMODULE_H
/*>>>>>USERMODULE_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file usermodule.h Módulo de usuarias y autenticación
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
/*<<<<<USERMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "usermastertable.h"

class QAction;
class QMenu;
/*>>>>>USERMODULE_INCLUDES*/

#include "userrecusuaria.h"

namespace gong {
namespace user {

/*<<<<<USERMODULE_CLASS_DEFINITION*/
class UserModule: public QObject, public dbModule
{
	Q_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto
public:
	UserModule();
	virtual ~UserModule();
	virtual bool initMainWindow(class MainWindow *win);
	virtual bool initDatabase(dbDefinition *dbdef);
	virtual bool login(FrmLogin *frmlogin, const Xtring &version, Xtring &addTitle, bool startingapp = true);
	virtual dbRecord *createRecord(const Xtring &tablename, dbRecordID recid=0, dbUser *user=0);
	virtual FrmEditRec *createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm = 0,
		FrmEditRec::EditMode editmode = DataTable::defaulteditmode,
		dbApplication::EditFlags editflags = dbApplication::editNone,
		QWidget *parent = 0, const char* name = 0,
		WidgetFlags fl = WidgetFlags(0) );
	virtual FrmEditRecDetail *createEditDetailForm(
		FrmEditRecMaster *frmmaster, int ndetail,
		dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm = 0,
		FrmEditRec::EditMode editmode = DataTable::defaulteditmode,
		dbApplication::EditFlags editflags = dbApplication::editNone,
		QWidget *parent = 0, const char* name = 0,
		WidgetFlags fl = WidgetFlags(0) );
/*>>>>>USERMODULE_CLASS_DEFINITION*/
/*<<<<<USERMODULE_RECORD_DEFINITIONS*/
public:
	user::MasterTable *getFicUsuaria() const { return pFicUsuaria; }
	user::MasterTable *getFicRol() const { return pFicRol; }

private:
	user::MasterTable *pFicUsuaria;
	user::MasterTable *pFicRol;
/*>>>>>USERMODULE_RECORD_DEFINITIONS*/
/*<<<<<USERMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuSystemUsuaria();
	void slotMenuSystemRol();
protected:
	QAction *pMenuSystemUsuaria;
	QAction *pMenuSystemRol;
/*>>>>>USERMODULE_MENU_DEFINITIONS*/
};

extern UserModule *ModuleInstance;
/*<<<<<USERMODULE_POSTAMBLE*/
} // namespace user
} // namespace gong
#endif // USER_USERMODULE_H
/*>>>>>USERMODULE_POSTAMBLE*/

