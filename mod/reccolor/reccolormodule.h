/*<<<<<COPYLEFT*/
/** @file reccolormodule.h Module RecColor
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
/*<<<<<RECCOLORMODULE_PREAMBLE*/
#ifndef _RECCOLOR_RECCOLORMODULE_H
#define _RECCOLOR_RECCOLORMODULE_H
/*>>>>>RECCOLORMODULE_PREAMBLE*/

/*<<<<<RECCOLORMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "reccolormastertable.h"

class QAction;
class QMenu;
/*>>>>>RECCOLORMODULE_INCLUDES*/

namespace gong {
namespace reccolor {

class ColorFrmEditRecBehavior;

/*<<<<<RECCOLORMODULE_CLASS_DEFINITION*/
class RecColorModule: public QObject, public dbModule
{
	Q_OBJECT
public:
	RecColorModule();
	virtual ~RecColorModule();
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
/*>>>>>RECCOLORMODULE_CLASS_DEFINITION*/
    virtual void afterLoad();
    virtual void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec );
protected:
    XtringList mTables;
    class ColorRecordBehavior *pColorRecordBehavior;
};

extern RecColorModule *ModuleInstance;

/*<<<<<RECCOLORMODULE_POSTAMBLE*/
} // namespace reccolor
} // namespace gong
#endif // RECCOLOR_RECCOLORMODULE_H
/*>>>>>RECCOLORMODULE_POSTAMBLE*/

