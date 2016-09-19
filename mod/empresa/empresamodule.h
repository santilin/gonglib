/*<<<<<COPYLEFT*/
/** @file empresamodule.h Módulo de empresas comerciales de GongLib
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
/*<<<<<EMPRESAMODULE_PREAMBLE*/
#ifndef _EMPRESA_EMPRESAMODULE_H
#define _EMPRESA_EMPRESAMODULE_H
/*>>>>>EMPRESAMODULE_PREAMBLE*/

/*<<<<<EMPRESAMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "empresamastertable.h"

class QAction;
class QMenu;
#ifdef HAVE_CONTACTOSMODULE
#include <contactosmodule.h>
#else
#error El módulo 'empresa::EmpresaModule' requiere el módulo 'Contactos'
#endif
/*>>>>>EMPRESAMODULE_INCLUDES*/
#include "empresarecempresa.h"

namespace gong {
namespace empresa {

/*<<<<<EMPRESAMODULE_CLASS_DEFINITION*/
class EmpresaModule: public QObject, public dbModule
{
	Q_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto
public:
	EmpresaModule();
	virtual ~EmpresaModule();
	virtual bool initMainWindow(class MainWindow *win);
	virtual bool initDatabase(dbDefinition *dbdef);
	virtual bool login(const Xtring &version, Xtring &addTitle, bool startingapp = true);
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
/*>>>>>EMPRESAMODULE_CLASS_DEFINITION*/
    QMenu *getMenuEmpresa() const {
        return pMenuEmpresa;
    }
    virtual void afterLoad(); // from dbModule
    virtual Variant getGlobalProperty(const Xtring &propertyname) const; // from dbModule
    int getMaxContador() const;
    void addContadorTable( const Xtring &tablename );

    void setCodEmpresa( int cod );
    int getCodEmpresa() const;
    dbRecordID getEmpresaID() const {
        return pRecEmpresa ? pRecEmpresa->getRecordID() : 0 ;
    }
    void setEjercicio( int ejercicio ) {
        mEjercicio = ejercicio;
    }
    int getEjercicio() const {
        return mEjercicio;
    }
    int getEjercicios(IntList &ejercicios) const;
    Xtring getNombreEmpresa() const;
    RecEmpresa *getRecEmpresa() const;
    bool isReadOnly() const;
    uint getDecimalesMoneda() const;
    bool usaProyectos() const;
    void rereadEmpresa();

private:
    RecEmpresa *pRecEmpresa;
    int mCodEmpresa, mEjercicio;
    XtringList mContadorTables;

    /*<<<<<EMPRESAMODULE_RECORD_DEFINITIONS*/
public:
	empresa::MasterTable *getFicEmpresa() const { return pFicEmpresa; }
	empresa::MasterTable *getFicDepartamento() const { return pFicDepartamento; }
	empresa::MasterTable *getFicMoneda() const { return pFicMoneda; }
	empresa::MasterTable *getFicTipoIVA() const { return pFicTipoIVA; }
	empresa::MasterTable *getFicProyecto() const { return pFicProyecto; }

private:
	empresa::MasterTable *pFicEmpresa;
	empresa::MasterTable *pFicDepartamento;
	empresa::MasterTable *pFicMoneda;
	empresa::MasterTable *pFicTipoIVA;
	empresa::MasterTable *pFicProyecto;
/*>>>>>EMPRESAMODULE_RECORD_DEFINITIONS*/
    /*<<<<<EMPRESAMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuEmpresaEmpresa();
	void slotMenuEmpresaDepartamento();
	void slotMenuEmpresaMoneda();
	void slotMenuEmpresaTipoIVA();
	void slotMenuEmpresaProyecto();
protected:
	QAction *pMenuEmpresaEmpresa;
	QAction *pMenuEmpresaDepartamento;
	QAction *pMenuEmpresaMoneda;
	QAction *pMenuEmpresaTipoIVA;
	QAction *pMenuEmpresaProyecto;
/*>>>>>EMPRESAMODULE_MENU_DEFINITIONS*/
    QMenu *pMenuEmpresa;
    QAction *pMenuEmpresaCambiarEjercicio, *pMenuEmpresaEjercicioAnterior, *pMenuEmpresaEjercicioSiguiente;

private slots:
    void slotMenuEmpresaCambiarEjercicio();
    void slotMenuEmpresaEjercicioAnterior();
    void slotMenuEmpresaEjercicioSiguiente();

    /*<<<<<EMPRESAMODULE_REQUIRES*/
public:
	contactos::ContactosModule *getContactosModule() const { return pContactosModule; }
private:
	contactos::ContactosModule *pContactosModule;
/*>>>>>EMPRESAMODULE_REQUIRES*/
};

extern EmpresaModule *ModuleInstance;

/*<<<<<EMPRESAMODULE_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESAMODULE_H
/*>>>>>EMPRESAMODULE_POSTAMBLE*/

