/*<<<<<COPYLEFT*/
/** @file editorialmodule.h Módulo para la gestión de los editorial
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
/*<<<<<EDITORIALMODULE_PREAMBLE*/
#ifndef _EDITORIAL_EDITORIALMODULE_H
#define _EDITORIAL_EDITORIALMODULE_H
/*>>>>>EDITORIALMODULE_PREAMBLE*/

/*<<<<<EDITORIALMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "editorialmastertable.h"

class QAction;
class QMenu;
#ifdef HAVE_EMPRESAMODULE
#include <empresamodule.h>
#else
#error El módulo 'editorial::EditorialModule' requiere el módulo 'Empresa'
#endif
#ifdef HAVE_FACTUMODULE
#include <factumodule.h>
#else
#error El módulo 'editorial::EditorialModule' requiere el módulo 'Factu'
#endif
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif
/*>>>>>EDITORIALMODULE_INCLUDES*/
#include <factumastertable.h>

namespace gong {

namespace factu {
class RecFacturaVenta;
}

namespace editorial {

class FrmEditFamiliaBehavior;

/*<<<<<EDITORIALMODULE_CLASS_DEFINITION*/
class EditorialModule: public QObject, public dbModule
{
	Q_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto
public:
	EditorialModule();
	virtual ~EditorialModule();
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
/*>>>>>EDITORIALMODULE_CLASS_DEFINITION*/
	void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec ); // from dbModule

/*<<<<<EDITORIALMODULE_RECORD_DEFINITIONS*/
public:
	editorial::MasterTable *getFicLibro() const { return pFicLibro; }
	editorial::MasterTable *getFicEditorial() const { return pFicEditorial; }

private:
	editorial::MasterTable *pFicLibro;
	editorial::MasterTable *pFicEditorial;
/*>>>>>EDITORIALMODULE_RECORD_DEFINITIONS*/

/*<<<<<EDITORIALMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuEmpresaEditorial();
protected:
	QAction *pMenuEmpresaEditorial;
/*>>>>>EDITORIALMODULE_MENU_DEFINITIONS*/
	QMenu *pMenuEditorial;

/*<<<<<EDITORIALMODULE_REQUIRES*/
public:
	empresa::EmpresaModule *getEmpresaModule() const { return pEmpresaModule; }
	factu::FactuModule *getFactuModule() const { return pFactuModule; }
#ifdef HAVE_CONTABMODULE
	contab::ContabModule *getContabModule() const { return pContabModule; }
#endif
private:
	empresa::EmpresaModule *pEmpresaModule;
	factu::FactuModule *pFactuModule;
#ifdef HAVE_CONTABMODULE
	contab::ContabModule *pContabModule;
#endif
/*>>>>>EDITORIALMODULE_REQUIRES*/
};

extern EditorialModule *ModuleInstance;

/*<<<<<EDITORIALMODULE_POSTAMBLE*/
} // namespace editorial
} // namespace gong
#endif // EDITORIAL_EDITORIALMODULE_H
/*>>>>>EDITORIALMODULE_POSTAMBLE*/

