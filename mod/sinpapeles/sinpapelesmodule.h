/*<<<<<COPYLEFT*/
/** @file sinpapelesmodule.h Module SinPapeles
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
/*<<<<<SINPAPELESMODULE_PREAMBLE*/
#ifndef _SINPAPELES_SINPAPELESMODULE_H
#define _SINPAPELES_SINPAPELESMODULE_H
/*>>>>>SINPAPELESMODULE_PREAMBLE*/

/*<<<<<SINPAPELESMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "sinpapelesmastertable.h"

class QAction;
class QMenu;
/*>>>>>SINPAPELESMODULE_INCLUDES*/

namespace gong {
namespace sinpapeles {

class SinPapelesRecordBehavior;
class SinPapelesFrmEditRecBehavior;

/*<<<<<SINPAPELESMODULE_CLASS_DEFINITION*/
class SinPapelesModule: public QObject, public dbModule
{
	Q_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto
public:
	SinPapelesModule();
	virtual ~SinPapelesModule();
	virtual bool initMainWindow(class MainWindow *win);
	virtual bool initDatabase(dbDefinition *dbdef);
	virtual bool login(const Xtring &version, Xtring &addTitle, bool startingapp = true);
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
/*>>>>>SINPAPELESMODULE_CLASS_DEFINITION*/
    virtual void afterLoad();
    virtual void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec );
protected:
    XtringList mTables;
    SinPapelesRecordBehavior *pSinPapelesRecordBehavior;
};

extern SinPapelesModule *ModuleInstance;

/*<<<<<SINPAPELESMODULE_POSTAMBLE*/
} // namespace sinpapeles
} // namespace gong
#endif // SINPAPELES_SINPAPELESMODULE_H
/*>>>>>SINPAPELESMODULE_POSTAMBLE*/

