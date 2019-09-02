/*<<<<<COPYLEFT*/
/** @file produccionmodule.h Module produccion
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
/*<<<<<PRODUCCIONMODULE_PREAMBLE*/
#ifndef _PRODUCCION_PRODUCCIONMODULE_H
#define _PRODUCCION_PRODUCCIONMODULE_H
/*>>>>>PRODUCCIONMODULE_PREAMBLE*/

/*<<<<<PRODUCCIONMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "produccionmastertable.h"

class QAction;
class QMenu;
/*>>>>>PRODUCCIONMODULE_INCLUDES*/

namespace gong {
namespace produccion {

class FrmEditArticuloBehavior;

/*<<<<<PRODUCCIONMODULE_CLASS_DEFINITION*/
class ProduccionModule: public QObject, public dbModule
{
	Q_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto
public:
	ProduccionModule();
	virtual ~ProduccionModule();
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
/*>>>>>PRODUCCIONMODULE_CLASS_DEFINITION*/

/*<<<<<PRODUCCIONMODULE_RECORD_DEFINITIONS*/
public:
	produccion::MasterTable *getFicLote() const { return pFicLote; }
	produccion::MasterTable *getFicLoteDet() const { return pFicLoteDet; }
	produccion::MasterTable *getFicArticuloComponente() const { return pFicArticuloComponente; }

private:
	produccion::MasterTable *pFicLote;
	produccion::MasterTable *pFicLoteDet;
	produccion::MasterTable *pFicArticuloComponente;
/*>>>>>PRODUCCIONMODULE_RECORD_DEFINITIONS*/

/*<<<<<PRODUCCIONMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuFacturacionLote();
protected:
	QAction *pMenuFacturacionLote;
/*>>>>>PRODUCCIONMODULE_MENU_DEFINITIONS*/
	virtual void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec ); // from dbModule
};

/*<<<<<PRODUCCIONMODULE_POSTAMBLE*/
} // namespace produccion
} // namespace gong
#endif // PRODUCCION_PRODUCCIONMODULE_H
/*>>>>>PRODUCCIONMODULE_POSTAMBLE*/
