/*<<<<<SOCIASMODULE_PREAMBLE*/
#ifndef _SOCIAS_SOCIASMODULE_H
#define _SOCIAS_SOCIASMODULE_H
/*>>>>>SOCIASMODULE_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file sociasmodule.h Plugin de gonglib para la gestión de las socias
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
/*<<<<<SOCIASMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "sociasmastertable.h"

class QAction;
class QMenu;

class NamesListTable;
#ifdef HAVE_EMPRESAMODULE
#include <empresamodule.h>
#else
#error El módulo 'socias::SociasModule' requiere el módulo 'Empresa'
#endif
#ifdef HAVE_CONTACTOSMODULE
#include <contactosmodule.h>
#else
#error El módulo 'socias::SociasModule' requiere el módulo 'Contactos'
#endif
#ifdef HAVE_PAGOSMODULE
#include <pagosmodule.h>
#else
#error El módulo 'socias::SociasModule' requiere el módulo 'Pagos'
#endif
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif
#ifdef HAVE_FACTUMODULE
#include <factumodule.h>
#endif
/*>>>>>SOCIASMODULE_INCLUDES*/

namespace gong {
namespace socias {

/*<<<<<SOCIASMODULE_CLASS_DEFINITION*/
class SociasModule: public QObject, public dbModule
{
	Q_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto
public:
	SociasModule();
	virtual ~SociasModule();
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
/*>>>>>SOCIASMODULE_CLASS_DEFINITION*/
	virtual void afterLoad(); // from dbModule

    enum SociasEstado { activo = 1, inactivo };
    enum Periodicidades { puntual = 1, diaria, semanal, quincenal, mensual, bimensual, trimestral,
                          cuatrimestral, semestral, anual, bianual
                        };

    bool hasContabilidad() const;
    virtual void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec ); // from dbModule

    /*<<<<<SOCIASMODULE_RECORD_DEFINITIONS*/
public:
	socias::MasterTable *getFicProyecto() const { return pFicProyecto; }
	socias::MasterTable *getFicMiembro() const { return pFicMiembro; }
	socias::MasterTable *getFicTipoSocia() const { return pFicTipoSocia; }
	socias::MasterTable *getFicPartidaProyecto() const { return pFicPartidaProyecto; }
	NamesListTable *pFicSociasEstado;
	NamesListTable *pFicPeriodicidad;

private:
	socias::MasterTable *pFicProyecto;
	socias::MasterTable *pFicMiembro;
	socias::MasterTable *pFicTipoSocia;
	socias::MasterTable *pFicPartidaProyecto;
/*>>>>>SOCIASMODULE_RECORD_DEFINITIONS*/

    /*<<<<<SOCIASMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuSociasProyecto();
	void slotMenuSociasMiembro();
	void slotMenuSociasTipoSocia();
	void slotMenuSociasPartidaProyecto();
protected:
	QAction *pMenuSociasProyecto;
	QAction *pMenuSociasMiembro;
	QAction *pMenuSociasTipoSocia;
	QAction *pMenuSociasPartidaProyecto;
/*>>>>>SOCIASMODULE_MENU_DEFINITIONS*/
    /*<<<<<SOCIASMODULE_REQUIRES*/
public:
	empresa::EmpresaModule *getEmpresaModule() const { return pEmpresaModule; }
	contactos::ContactosModule *getContactosModule() const { return pContactosModule; }
	pagos::PagosModule *getPagosModule() const { return pPagosModule; }
#ifdef HAVE_CONTABMODULE
	contab::ContabModule *getContabModule() const { return pContabModule; }
#endif
#ifdef HAVE_FACTUMODULE
	factu::FactuModule *getFactuModule() const { return pFactuModule; }
#endif
private:
	empresa::EmpresaModule *pEmpresaModule;
	contactos::ContactosModule *pContactosModule;
	pagos::PagosModule *pPagosModule;
#ifdef HAVE_CONTABMODULE
	contab::ContabModule *pContabModule;
#endif
#ifdef HAVE_FACTUMODULE
	factu::FactuModule *pFactuModule;
#endif
/*>>>>>SOCIASMODULE_REQUIRES*/

private slots:
    void slotMenuGenerarRecibos();
private:
    QMenu *pMenuSocias;
    QAction *pMenuGenerarRecibos;
};

extern SociasModule *ModuleInstance;

/*<<<<<SOCIASMODULE_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASMODULE_H
/*>>>>>SOCIASMODULE_POSTAMBLE*/

