/*<<<<<COPYLEFT*/
/** @file pagosmodule.h Module pagos
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
/*<<<<<PAGOSMODULE_PREAMBLE*/
#ifndef _PAGOS_PAGOSMODULE_H
#define _PAGOS_PAGOSMODULE_H
/*>>>>>PAGOSMODULE_PREAMBLE*/

/*<<<<<PAGOSMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "pagosmastertable.h"

class QAction;
class QMenu;

class NamesListTable;
#ifdef HAVE_EMPRESAMODULE
#include <empresamodule.h>
#else
#error El módulo 'pagos::PagosModule' requiere el módulo 'Empresa'
#endif
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif
#ifdef HAVE_TESORERIAMODULE
#include <tesoreriamodule.h>
#endif
/*>>>>>PAGOSMODULE_INCLUDES*/

namespace gong {

namespace empresa {
class RecProyecto;
}

namespace pagos {

class RecRemesaCobro;

/*<<<<<PAGOSMODULE_CLASS_DEFINITION*/
class PagosModule: public QObject, public dbModule
{
	Q_OBJECT
public:
	PagosModule();
	virtual ~PagosModule();
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
/*>>>>>PAGOSMODULE_CLASS_DEFINITION*/

    enum EstadoRecibo { ReciboPendiente = 1, ReciboPagado = 2, ReciboDevuelto = 3, ReciboAnulado = 4 };
    virtual Xtring getMigrationSQL( uint old_version ) const;
    empresa::RecProyecto *getRecProyectoPadre(const FrmEditRec *childfrm) const;
    RecRemesaCobro *getRecRemesaCobroPadre(const FrmEditRec *childfrm) const;

/*<<<<<PAGOSMODULE_RECORD_DEFINITIONS*/
public:
	pagos::MasterTable *getFicFormaPago() const { return pFicFormaPago; }
	pagos::MasterTable *getFicRemesaCobro() const { return pFicRemesaCobro; }
	pagos::MasterTable *getFicCobro() const { return pFicCobro; }
	pagos::MasterTable *getFicPago() const { return pFicPago; }
	NamesListTable *pFicTipoFormaPago;
	NamesListTable *pFicEstadoRecibo;

private:
	pagos::MasterTable *pFicFormaPago;
	pagos::MasterTable *pFicRemesaCobro;
	pagos::MasterTable *pFicCobro;
	pagos::MasterTable *pFicPago;
/*>>>>>PAGOSMODULE_RECORD_DEFINITIONS*/
    /*<<<<<PAGOSMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuEmpresaFormaPago();
	void slotMenuPagosRemesaCobro();
	void slotMenuPagosCobro();
	void slotMenuPagosPago();
protected:
	QAction *pMenuEmpresaFormaPago;
	QAction *pMenuPagosRemesaCobro;
	QAction *pMenuPagosCobro;
	QAction *pMenuPagosPago;
/*>>>>>PAGOSMODULE_MENU_DEFINITIONS*/

public:
    static Xtring sLastCuentaPago, sLastDocumentoPago, sLastMonedaCodigo;
    static Date sLastFechaPago;

    /*<<<<<PAGOSMODULE_REQUIRES*/
public:
	empresa::EmpresaModule *getEmpresaModule() const { return pEmpresaModule; }
#ifdef HAVE_CONTABMODULE
	contab::ContabModule *getContabModule() const { return pContabModule; }
#endif
#ifdef HAVE_TESORERIAMODULE
	tesoreria::TesoreriaModule *getTesoreriaModule() const { return pTesoreriaModule; }
#endif
private:
	empresa::EmpresaModule *pEmpresaModule;
#ifdef HAVE_CONTABMODULE
	contab::ContabModule *pContabModule;
#endif
#ifdef HAVE_TESORERIAMODULE
	tesoreria::TesoreriaModule *pTesoreriaModule;
#endif
/*>>>>>PAGOSMODULE_REQUIRES*/
};

extern PagosModule *ModuleInstance;

/*<<<<<PAGOSMODULE_POSTAMBLE*/
} // namespace pagos
} // namespace gong
#endif // PAGOS_PAGOSMODULE_H
/*>>>>>PAGOSMODULE_POSTAMBLE*/

