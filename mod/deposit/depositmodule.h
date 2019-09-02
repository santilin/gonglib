/*<<<<<COPYLEFT*/
/** @file depositmodule.h Module deposit
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
/*<<<<<DEPOSITMODULE_PREAMBLE*/
#ifndef _DEPOSIT_DEPOSITMODULE_H
#define _DEPOSIT_DEPOSITMODULE_H
/*>>>>>DEPOSITMODULE_PREAMBLE*/

/*<<<<<DEPOSITMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "depositmastertable.h"

class QAction;
class QMenu;
#ifdef HAVE_EMPRESAMODULE
#include <empresamodule.h>
#else
#error El módulo 'deposit::DepositModule' requiere el módulo 'Empresa'
#endif
#ifdef HAVE_FACTUMODULE
#include <factumodule.h>
#else
#error El módulo 'deposit::DepositModule' requiere el módulo 'Factu'
#endif
#ifdef HAVE_TESORERIAMODULE
#include <tesoreriamodule.h>
#endif
/*>>>>>DEPOSITMODULE_INCLUDES*/
#include <facturecfacturaventa.h>

namespace gong {
namespace deposit {

/*<<<<<DEPOSITMODULE_CLASS_DEFINITION*/
class DepositModule: public QObject, public dbModule
{
	Q_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto
public:
	DepositModule();
	virtual ~DepositModule();
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
/*>>>>>DEPOSITMODULE_CLASS_DEFINITION*/
	virtual void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec ); // from dbModule

/*<<<<<DEPOSITMODULE_RECORD_DEFINITIONS*/
public:
	deposit::MasterTable *getFicLiquidacionVenta() const { return pFicLiquidacionVenta; }
	deposit::MasterTable *getFicLiquidacionVentaDet() const { return pFicLiquidacionVentaDet; }
	deposit::MasterTable *getFicLiquidacionCompra() const { return pFicLiquidacionCompra; }
	deposit::MasterTable *getFicLiquidacionCompraDet() const { return pFicLiquidacionCompraDet; }

private:
	deposit::MasterTable *pFicLiquidacionVenta;
	deposit::MasterTable *pFicLiquidacionVentaDet;
	deposit::MasterTable *pFicLiquidacionCompra;
	deposit::MasterTable *pFicLiquidacionCompraDet;
/*>>>>>DEPOSITMODULE_RECORD_DEFINITIONS*/

protected slots:
	void slotMenuLiquidacionDepositosCompra();
	void slotMenuLiquidacionDepositosVenta();
	void slotMenuLiquidacionFacturarLiquidacionesVenta();
	void slotMenuLiquidacionInformeDepositos();

/*<<<<<DEPOSITMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuDepositosLiquidacionVenta();
	void slotMenuDepositosLiquidacionCompra();
protected:
	QAction *pMenuDepositosLiquidacionVenta;
	QAction *pMenuDepositosLiquidacionCompra;
/*>>>>>DEPOSITMODULE_MENU_DEFINITIONS*/
	QMenu *pMenuDepositos;
	QAction *pMenuLiquidacionDepositosCompra;
	QAction *pMenuLiquidacionDepositosVenta;
	QAction *pMenuLiquidacionVenta;
	QAction *pMenuLiquidacionCompra;
	QAction *pMenuFacturacionInformeDepositos;

/*<<<<<DEPOSITMODULE_REQUIRES*/
public:
	empresa::EmpresaModule *getEmpresaModule() const { return pEmpresaModule; }
	factu::FactuModule *getFactuModule() const { return pFactuModule; }
#ifdef HAVE_TESORERIAMODULE
	tesoreria::TesoreriaModule *getTesoreriaModule() const { return pTesoreriaModule; }
#endif
private:
	empresa::EmpresaModule *pEmpresaModule;
	factu::FactuModule *pFactuModule;
#ifdef HAVE_TESORERIAMODULE
	tesoreria::TesoreriaModule *pTesoreriaModule;
#endif
/*>>>>>DEPOSITMODULE_REQUIRES*/
};

extern DepositModule *ModuleInstance;

/*<<<<<DEPOSITMODULE_POSTAMBLE*/
} // namespace deposit
} // namespace gong
#endif // DEPOSIT_DEPOSITMODULE_H
/*>>>>>DEPOSITMODULE_POSTAMBLE*/
