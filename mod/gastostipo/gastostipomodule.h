/*<<<<<COPYLEFT*/
/** @file gastostipomodule.h Module gastos
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
/*<<<<<GASTOSTIPOMODULE_PREAMBLE*/
#ifndef _GASTOSTIPO_GASTOSTIPOMODULE_H
#define _GASTOSTIPO_GASTOSTIPOMODULE_H
/*>>>>>GASTOSTIPOMODULE_PREAMBLE*/

/*<<<<<GASTOSTIPOMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "gastostipomastertable.h"

class QAction;
class QMenu;

class NamesListTable;
#ifdef HAVE_PAGOSMODULE
#include <pagosmodule.h>
#endif
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif
#ifdef HAVE_FACTUMODULE
#include <factumodule.h>
#endif
/*>>>>>GASTOSTIPOMODULE_INCLUDES*/

namespace gong {
namespace gastostipo {

/*<<<<<GASTOSTIPOMODULE_CLASS_DEFINITION*/
class GastosTipoModule: public QObject, public dbModule
{
	Q_OBJECT
public:
	GastosTipoModule();
	virtual ~GastosTipoModule();
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
/*>>>>>GASTOSTIPOMODULE_CLASS_DEFINITION*/
/*<<<<<GASTOSTIPOMODULE_RECORD_DEFINITIONS*/
public:
	gastostipo::MasterTable *getFicGastoTipo() const { return pFicGastoTipo; }
	NamesListTable *pFicPedirCampo;
	NamesListTable *pFicCategoriaGasto;

private:
	gastostipo::MasterTable *pFicGastoTipo;
/*>>>>>GASTOSTIPOMODULE_RECORD_DEFINITIONS*/
/*<<<<<GASTOSTIPOMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuGastosTipoGastoTipo();
protected:
	QAction *pMenuGastosTipoGastoTipo;
/*>>>>>GASTOSTIPOMODULE_MENU_DEFINITIONS*/
	QMenu *pMenuGastosTipo;
/*<<<<<GASTOSTIPOMODULE_REQUIRES*/
public:
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
#ifdef HAVE_PAGOSMODULE
	pagos::PagosModule *pPagosModule;
#endif
#ifdef HAVE_CONTABMODULE
	contab::ContabModule *pContabModule;
#endif
#ifdef HAVE_FACTUMODULE
	factu::FactuModule *pFactuModule;
#endif
/*>>>>>GASTOSTIPOMODULE_REQUIRES*/
};

extern GastosTipoModule *ModuleInstance;

/*<<<<<GASTOSTIPOMODULE_POSTAMBLE*/
} // namespace gastostipo
} // namespace gong
#endif // GASTOSTIPO_GASTOSTIPOMODULE_H
/*>>>>>GASTOSTIPOMODULE_POSTAMBLE*/

