/*<<<<<ECOTIENDAMODULE_PREAMBLE*/
#ifndef _ECOTIENDA_ECOTIENDAMODULE_H
#define _ECOTIENDA_ECOTIENDAMODULE_H
/*>>>>>ECOTIENDAMODULE_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file ecotiendamodule.h Module ecotienda
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
/*<<<<<ECOTIENDAMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "ecotiendamastertable.h"

class QAction;
class QMenu;
#ifdef HAVE_FACTUMODULE
#include <factumodule.h>
#else
#error El módulo 'ecotienda::EcoTiendaModule' requiere el módulo 'Factu'
#endif
/*>>>>>ECOTIENDAMODULE_INCLUDES*/

namespace gong {
namespace ecotienda {

/*<<<<<ECOTIENDAMODULE_CLASS_DEFINITION*/
class EcoTiendaModule: public QObject, public dbModule
{
	Q_OBJECT
public:
	EcoTiendaModule();
	virtual ~EcoTiendaModule();
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
/*>>>>>ECOTIENDAMODULE_CLASS_DEFINITION*/
	virtual void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec ); // from dbModule
/*<<<<<ECOTIENDAMODULE_REQUIRES*/
public:
	factu::FactuModule *getFactuModule() const { return pFactuModule; }
private:
	factu::FactuModule *pFactuModule;
/*>>>>>ECOTIENDAMODULE_REQUIRES*/
};

extern EcoTiendaModule *ModuleInstance;
/*<<<<<ECOTIENDAMODULE_POSTAMBLE*/
} // namespace ecotienda
} // namespace gong
#endif // ECOTIENDA_ECOTIENDAMODULE_H
/*>>>>>ECOTIENDAMODULE_POSTAMBLE*/

