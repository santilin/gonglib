/*<<<<<CONTACTOSMODULE_PREAMBLE*/
#ifndef _CONTACTOS_CONTACTOSMODULE_H
#define _CONTACTOS_CONTACTOSMODULE_H
/*>>>>>CONTACTOSMODULE_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file contactosmodule.h Plugin para los contactos de gestiong
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

/*<<<<<CONTACTOSMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "contactosmastertable.h"

class QAction;
class QMenu;

class NamesListTable;
/*>>>>>CONTACTOSMODULE_INCLUDES*/

namespace gong {
namespace contactos {

/*<<<<<CONTACTOSMODULE_CLASS_DEFINITION*/
class ContactosModule: public QObject, public dbModule
{
	Q_OBJECT
public:
	ContactosModule();
	virtual ~ContactosModule();
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
/*>>>>>CONTACTOSMODULE_CLASS_DEFINITION*/
    QAction *getMenuContactos() const {
        return pMenuContactosContacto;
    }
    void afterLoad(); // from dbModule

    /*<<<<<CONTACTOSMODULE_RECORD_DEFINITIONS*/
public:
	contactos::MasterTable *getFicContacto() const { return pFicContacto; }
	NamesListTable *pFicTratamientoContacto;

private:
	contactos::MasterTable *pFicContacto;
/*>>>>>CONTACTOSMODULE_RECORD_DEFINITIONS*/
    contactos::MasterTable *pFicContactoImagen;

    /*<<<<<CONTACTOSMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuContactosContacto();
protected:
	QAction *pMenuContactosContacto;
/*>>>>>CONTACTOSMODULE_MENU_DEFINITIONS*/
private slots:
    void slotMenuContactosMailing();
protected:
    QAction *pMenuContactosMailing;
};

extern ContactosModule *ModuleInstance;

/*<<<<<CONTACTOSMODULE_POSTAMBLE*/
} // namespace contactos
} // namespace gong
#endif // CONTACTOS_CONTACTOSMODULE_H
/*>>>>>CONTACTOSMODULE_POSTAMBLE*/

