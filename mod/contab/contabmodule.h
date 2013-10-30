/*<<<<<COPYLEFT*/
/** @file contabmodule.h Plugin de contabilidad para gestiong
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<CONTABMODULE_PREAMBLE*/
#ifndef _CONTAB_CONTABMODULE_H
#define _CONTAB_CONTABMODULE_H
/*>>>>>CONTABMODULE_PREAMBLE*/

/*<<<<<CONTABMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "contabmastertable.h"

class QAction;
class QMenu;

class NamesListTable;
#ifdef HAVE_EMPRESAMODULE
#include <empresamodule.h>
#else
#error El módulo 'contab::ContabModule' requiere el módulo 'Empresa'
#endif
/*>>>>>CONTABMODULE_INCLUDES*/


namespace gong {
namespace contab {

class FrmEditTipoIVABehavior;
class RecAsiento;
class RecCuenta;

/*<<<<<CONTABMODULE_CLASS_DEFINITION*/
class ContabModule: public QObject, public dbModule
{
    Q_OBJECT
public:
    ContabModule();
    virtual ~ContabModule();
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
    /*>>>>>CONTABMODULE_CLASS_DEFINITION*/
    virtual void afterLoad(); // from dbModule
    virtual void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec ); // from dbModule

    int RecalcularSaldosCuentas( int ejercicio );
    XtringList &getListaTiposCuenta() {
        return mListaTiposCuenta;
    }
    int getDigitosTrabajo() const;
    uint getMaxNivelGrupo() const;
    contab::MasterTable *getFicheroApuntes() const {
        return pFicApunte;
    }
    RecAsiento *creaAsientoSimple(FrmEditRec *parent, RecAsiento *old_asiento, Date fecha,
                                  const Xtring &rec_origen, dbRecordID proyecto_id, const Xtring &descripcion,
                                  const Xtring &concepto,  const Xtring &cuentadebe, Money debe,
                                  const Xtring &cuentahaber, Money haber, bool supervisar );
    RecAsiento *creaAsientoSimple(FrmEditRec *parent, RecAsiento *old_asiento, Date fecha,
                                  const Xtring &rec_origen, dbRecordID proyecto_id, const Xtring &descripcion,
                                  const Xtring &concepto,  dbRecordID cuentadebe, Money debe,
                                  dbRecordID cuentahaber, Money haber, bool supervisar );
    typedef struct {
        Xtring cuentadebe, concepto;
        Money debe, haber;
        Xtring desc_cuenta_debe, contrapartida;
    } ApunteQuickValues;
    RecAsiento *creaAsientoMultiple(FrmEditRec *parent, RecAsiento *old_asiento, Date fecha,
                                    const Xtring &rec_origen, dbRecordID proyecto_id, const Xtring &descripcion,
                                    List<ApunteQuickValues> apuntevalues, bool supervisar );
    RecCuenta *creaCuentaTercero( FrmEditRec *parent, dbRecord *tercero,
                                  const Xtring &prefijo_cuenta, bool supervisar );
    bool isContabActive( empresa::RecEmpresa *recempresa = 0 ) const;

    /*<<<<<CONTABMODULE_RECORD_DEFINITIONS*/
public:
    contab::MasterTable *getFicCuenta() const {
        return pFicCuenta;
    }
    contab::MasterTable *getFicAsiento() const {
        return pFicAsiento;
    }
    contab::MasterTable *getFicApunte() const {
        return pFicApunte;
    }
    NamesListTable *pFicTipoAsiento;

private:
    contab::MasterTable *pFicCuenta;
    contab::MasterTable *pFicAsiento;
    contab::MasterTable *pFicApunte;
    /*>>>>>CONTABMODULE_RECORD_DEFINITIONS*/

private slots:
    void slotMenuContabilidadDiarioRecalcular();
    void slotMenuContabilidadCuentasManuales();
    /*<<<<<CONTABMODULE_MENU_DEFINITIONS*/
private slots:
    void slotMenuContabilidadCuenta();
    void slotMenuContabilidadAsiento();
protected:
    QAction *pMenuContabilidadCuenta;
    QAction *pMenuContabilidadAsiento;
    /*>>>>>CONTABMODULE_MENU_DEFINITIONS*/
    QAction *pMenuContabilidadDiarioRecalcular;
    QAction *pMenuContabilidadCuentasManuales;
protected:
    XtringList mListaTiposCuenta;
    QMenu *pMenuContabilidad;
    QAction *pMenuContabilidadExtractos;
    QMenu *pMenuContabilidadDiario;
    QAction *pMenuContabilidadDiarioRenumerar;
    QMenu *pMenuContabilidadAperturacierre;
    QAction *pMenuContabilidadAperturacierreCreaejerc;
    QAction *pMenuContabilidadAperturacierreCreaasieaper;
    QAction *pMenuContabilidadAperturacierreRegularizar;
    QAction *pMenuContabilidadAperturacierreCerrar;


#if 0
private:
    void slotMenuContabilidadExtractos();
    void slotMenuContabilidadDiarioRenumerar();
    void slotMenuContabilidadAperturacierreCreaejerc();
    void slotMenuContabilidadAperturacierreCreaasieaper();
    void slotMenuContabilidadAperturacierreRegularizar();
    void slotMenuContabilidadAperturacierreCerrar();
#endif
public:
    static int mLastAsientoID;
    static int mLastProyectoCodigo;
    static Xtring mLastDocumento;

    /*<<<<<CONTABMODULE_REQUIRES*/
public:
    empresa::EmpresaModule *getEmpresaModule() const {
        return pEmpresaModule;
    }
private:
    empresa::EmpresaModule *pEmpresaModule;
    /*>>>>>CONTABMODULE_REQUIRES*/
};

extern ContabModule *ModuleInstance;

/*<<<<<CONTABMODULE_POSTAMBLE*/
} // namespace contab
} // namespace gong
#endif // CONTAB_CONTABMODULE_H
/*>>>>>CONTABMODULE_POSTAMBLE*/


