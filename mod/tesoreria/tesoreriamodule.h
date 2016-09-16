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
#ifdef HAVE_EMPRESAMODULE
#include <empresamodule.h>
#else
#error El módulo 'tesoreria::TesoreriaModule' requiere el módulo 'Empresa'
#endif
/*>>>>>TESORERIAMODULE_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace tesoreria {

/*<<<<<TESORERIAMODULE_CLASS_DEFINITION*/
class TesoreriaModule: public QObject, public dbModule
{
	Q_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto
public:
	TesoreriaModule();
	virtual ~TesoreriaModule();
	virtual bool initMainWindow(class MainWindow *win);
	virtual bool initDatabase(dbDefinition *dbdef);
	virtual bool login( QDialog *frmlogin, const Xtring &version, Xtring &addTitle, bool startingapp = true);
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

	virtual void afterCreateEditForm(FrmEditRec* frm, dbRecord* rec); // from dbModule
    virtual void afterLoad(); // from dbModule
	
    XtringList &getTablasTerceros() {
        return mTablasTerceros;
    }
    XtringList &getTablasConceptos() {
        return mTablasConceptos;
    }
    XtringList &getTablasCuentas() {
        return mTablasCuentas;
    }
    XtringList &getTablasDocumentos() {
        return mTablasDocumentos;
    }

/*<<<<<TESORERIAMODULE_REQUIRES*/
public:
	empresa::EmpresaModule *getEmpresaModule() const { return pEmpresaModule; }
private:
	empresa::EmpresaModule *pEmpresaModule;
/*>>>>>TESORERIAMODULE_REQUIRES*/

/*<<<<<TESORERIAMODULE_RECORD_DEFINITIONS*/
public:
	tesoreria::MasterTable *getFicApunteTesoreria() const { return pFicApunteTesoreria; }
	tesoreria::MasterTable *getFicCuentaTesoreria() const { return pFicCuentaTesoreria; }
	tesoreria::MasterTable *getFicTerceroTesoreria() const { return pFicTerceroTesoreria; }
	tesoreria::MasterTable *getFicConceptoTesoreria() const { return pFicConceptoTesoreria; }
	tesoreria::MasterTable *getFicTipoApunteTesoreria() const { return pFicTipoApunteTesoreria; }

private:
	tesoreria::MasterTable *pFicApunteTesoreria;
	tesoreria::MasterTable *pFicCuentaTesoreria;
	tesoreria::MasterTable *pFicTerceroTesoreria;
	tesoreria::MasterTable *pFicConceptoTesoreria;
	tesoreria::MasterTable *pFicTipoApunteTesoreria;
/*>>>>>TESORERIAMODULE_RECORD_DEFINITIONS*/
    /*<<<<<TESORERIAMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuTesoreriaApunteTesoreria();
	void slotMenuTesoreriaCuentaTesoreria();
	void slotMenuTesoreriaTerceroTesoreria();
	void slotMenuTesoreriaConceptoTesoreria();
	void slotMenuTesoreriaTipoApunteTesoreria();
protected:
	QAction *pMenuTesoreriaApunteTesoreria;
	QAction *pMenuTesoreriaCuentaTesoreria;
	QAction *pMenuTesoreriaTerceroTesoreria;
	QAction *pMenuTesoreriaConceptoTesoreria;
	QAction *pMenuTesoreriaTipoApunteTesoreria;
/*>>>>>TESORERIAMODULE_MENU_DEFINITIONS*/
    QMenu *pMenuTesoreria;
    XtringList mTablasTerceros, mTablasConceptos, mTablasCuentas, mTablasDocumentos;
};

extern TesoreriaModule *ModuleInstance;

/*<<<<<TESORERIAMODULE_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIAMODULE_H
/*>>>>>TESORERIAMODULE_POSTAMBLE*/

