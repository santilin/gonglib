/*<<<<<COPYLEFT*/
/** @file gastosmodule.h Module gastos
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
/*<<<<<GASTOSMODULE_PREAMBLE*/
#ifndef _GASTOS_GASTOSMODULE_H
#define _GASTOS_GASTOSMODULE_H
/*>>>>>GASTOSMODULE_PREAMBLE*/

/*<<<<<GASTOSMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "gastosmastertable.h"

class QAction;
class QMenu;

class NamesListTable;
#ifdef HAVE_GASTOSMODULE
#include <gastosmodule.h>
#else
#error El módulo 'gastos::GastosModule' requiere el módulo 'Gastos'
#endif
#ifdef HAVE_PAGOSMODULE
#include <pagosmodule.h>
#endif
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif
#ifdef HAVE_FACTUMODULE
#include <factumodule.h>
#endif
/*>>>>>GASTOSMODULE_INCLUDES*/

namespace gong {
namespace gastos {

/*<<<<<GASTOSMODULE_CLASS_DEFINITION*/
class GastosModule: public QObject, public dbModule
{
	Q_OBJECT
public:
	GastosModule();
	virtual ~GastosModule();
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
/*>>>>>GASTOSMODULE_CLASS_DEFINITION*/
/*<<<<<GASTOSMODULE_RECORD_DEFINITIONS*/
public:
	gastos::MasterTable *getFicGasto() const { return pFicGasto; }
	NamesListTable *pFicTipoGasto;
	NamesListTable *pFicCategoriaGasto;

private:
	gastos::MasterTable *pFicGasto;
/*>>>>>GASTOSMODULE_RECORD_DEFINITIONS*/
/*<<<<<GASTOSMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuGastosGasto();
protected:
	QAction *pMenuGastosGasto;
/*>>>>>GASTOSMODULE_MENU_DEFINITIONS*/
	QMenu *pMenuGastos;
/*<<<<<GASTOSMODULE_REQUIRES*/
public:
	gastos::GastosModule *getGastosModule() const { return pGastosModule; }
#ifdef HAVE_PAGOSMODULE
	pagos::PagosModule *getPagosModule() const { return pPagosModule; }
#endif
#ifdef HAVE_CONTABMODULE
	contab::ContabModule *getContabModule() const { return pContabModule; }
#endif
#ifdef HAVE_FACTUMODULE
	factu::FactuModule *getFactuModule() const { return pFactuModule; }
#endif
private:
	gastos::GastosModule *pGastosModule;
#ifdef HAVE_PAGOSMODULE
	pagos::PagosModule *pPagosModule;
#endif
#ifdef HAVE_CONTABMODULE
	contab::ContabModule *pContabModule;
#endif
#ifdef HAVE_FACTUMODULE
	factu::FactuModule *pFactuModule;
#endif
/*>>>>>GASTOSMODULE_REQUIRES*/
};

extern GastosModule *ModuleInstance;

/*<<<<<GASTOSMODULE_POSTAMBLE*/
} // namespace gastos
} // namespace gong
#endif // GASTOS_GASTOSMODULE_H
/*>>>>>GASTOSMODULE_POSTAMBLE*/

