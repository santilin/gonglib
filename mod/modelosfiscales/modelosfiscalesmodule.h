/*<<<<<MODELOSFISCALESMODULE_PREAMBLE*/
#ifndef _MODELOSFISCALES_MODELOSFISCALESMODULE_H
#define _MODELOSFISCALES_MODELOSFISCALESMODULE_H
/*>>>>>MODELOSFISCALESMODULE_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file modelosfiscalesmodule.h Module modelosfiscales
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
/*<<<<<MODELOSFISCALESMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "modelosfiscalesmastertable.h"

class QAction;
class QMenu;
/*>>>>>MODELOSFISCALESMODULE_INCLUDES*/

namespace gong {
namespace modelosfiscales {

/*<<<<<MODELOSFISCALESMODULE_CLASS_DEFINITION*/
class ModelosFiscalesModule: public QObject, public dbModule
{
	Q_OBJECT
public:
	ModelosFiscalesModule();
	virtual ~ModelosFiscalesModule();
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
/*>>>>>MODELOSFISCALESMODULE_CLASS_DEFINITION*/
};

extern ModelosFiscalesModule *ModuleInstance;
/*<<<<<MODELOSFISCALESMODULE_POSTAMBLE*/
} // namespace modelosfiscales
} // namespace gong
#endif // MODELOSFISCALES_MODELOSFISCALESMODULE_H
/*>>>>>MODELOSFISCALESMODULE_POSTAMBLE*/

