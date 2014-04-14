/*<<<<<TESORERIAMODULE_PREAMBLE*/
#ifndef _TESORERIA_TESORERIAMODULE_H
#define _TESORERIA_TESORERIAMODULE_H
/*>>>>>TESORERIAMODULE_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file tesoreriamodule.h Module tesoreria
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
/*<<<<<TESORERIAMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "tesoreriamastertable.h"

class QAction;
class QMenu;

class NamesListTable;
/*>>>>>TESORERIAMODULE_INCLUDES*/

namespace gong {
namespace tesoreria {

/*<<<<<TESORERIAMODULE_CLASS_DEFINITION*/
class TesoreriaModule: public QObject, public dbModule
{
	Q_OBJECT
public:
	TesoreriaModule();
	virtual ~TesoreriaModule();
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
/*>>>>>TESORERIAMODULE_CLASS_DEFINITION*/
/*<<<<<TESORERIAMODULE_RECORD_DEFINITIONS*/
public:
	tesoreria::MasterTable *getFicCuentaTesoreria() const { return pFicCuentaTesoreria; }
	tesoreria::MasterTable *getFicApunteTesoreria() const { return pFicApunteTesoreria; }
	NamesListTable *pFicTipoApunteTesoreria;

private:
	tesoreria::MasterTable *pFicCuentaTesoreria;
	tesoreria::MasterTable *pFicApunteTesoreria;
/*>>>>>TESORERIAMODULE_RECORD_DEFINITIONS*/
/*<<<<<TESORERIAMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuTesoreriaCuentaTesoreria();
	void slotMenuTesoreriaApunteTesoreria();
protected:
	QAction *pMenuTesoreriaCuentaTesoreria;
	QAction *pMenuTesoreriaApunteTesoreria;
/*>>>>>TESORERIAMODULE_MENU_DEFINITIONS*/
	QMenu *pMenuTesoreria;
};

extern TesoreriaModule *ModuleInstance;

/*<<<<<TESORERIAMODULE_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIAMODULE_H
/*>>>>>TESORERIAMODULE_POSTAMBLE*/
