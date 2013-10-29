/*<<<<<COPYLEFT*/
/** @file factumodule.h Módulo de facturación de gonglib
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
/*<<<<<FACTUMODULE_PREAMBLE*/
#ifndef _FACTU_FACTUMODULE_H
#define _FACTU_FACTUMODULE_H
/*>>>>>FACTUMODULE_PREAMBLE*/

/*<<<<<FACTUMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "factumastertable.h"

class QAction;
class QMenu;

class NamesListTable;
#ifdef HAVE_EMPRESAMODULE
#include <empresamodule.h>
#else
#error El módulo 'factu::FactuModule' requiere el módulo 'Empresa'
#endif
#ifdef HAVE_PAGOSMODULE
#include <pagosmodule.h>
#else
#error El módulo 'factu::FactuModule' requiere el módulo 'Pagos'
#endif
#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif
/*>>>>>FACTUMODULE_INCLUDES*/
#include "factufrmeditempresabehavior.h"

#ifdef HAVE_CONTABMODULE
#include <contabmodule.h>
#endif

namespace gong {
namespace factu {

/*<<<<<FACTUMODULE_CLASS_DEFINITION*/
class FactuModule: public QObject, public dbModule
{
    Q_OBJECT
public:
    FactuModule();
    virtual ~FactuModule();
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
    /*>>>>>FACTUMODULE_CLASS_DEFINITION*/
    virtual void afterLoad();    // from dbModule
    virtual void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec ); // from dbModule
    virtual Xtring getMigrationSQL( uint old_version ) const;

// Cobros y pagos
    int getMaxContador(const Xtring &table, dbRecordID empresa_id, int ejercicio,
                       const Xtring &serie) const;
    QMenu *getMenuFacturacion() const {
        return pMenuFacturacion;
    }
    Date getWorkingDate();
    void setWorkingDate( Date dt );
    const XtringList &getInsertables() const {
        return mInsertables;
    }
    void addInsertable( const Xtring &tablename ) {
        mInsertables << tablename;
    }
    bool insertDetails( FrmEditRecMaster *masterform, FrmEditRecDetail *detailform,
                        const Xtring &excludedtables=Xtring::null,
                        const Xtring &excludedfields = Xtring::null);
    void calcImporteDetalle( dbRecord *detalle, bool redondeaimportes );
    enum EstadoPedido { PedidoPendiente = 1, PedidoPedido, PedidoRecibido, PedidoEntregado };

    /*<<<<<FACTUMODULE_RECORD_DEFINITIONS*/
public:
    factu::MasterTable *getFicArticulo() const {
        return pFicArticulo;
    }
    factu::MasterTable *getFicCliente() const {
        return pFicCliente;
    }
    factu::MasterTable *getFicProveedora() const {
        return pFicProveedora;
    }
    factu::MasterTable *getFicAgente() const {
        return pFicAgente;
    }
    factu::MasterTable *getFicFamilia() const {
        return pFicFamilia;
    }
    factu::MasterTable *getFicTipoDoc() const {
        return pFicTipoDoc;
    }
    factu::MasterTable *getFicPresupuestoVenta() const {
        return pFicPresupuestoVenta;
    }
    factu::MasterTable *getFicPresupuestoVentaDet() const {
        return pFicPresupuestoVentaDet;
    }
    factu::MasterTable *getFicPedidoVenta() const {
        return pFicPedidoVenta;
    }
    factu::MasterTable *getFicPedidoVentaDet() const {
        return pFicPedidoVentaDet;
    }
    factu::MasterTable *getFicAlbaranVenta() const {
        return pFicAlbaranVenta;
    }
    factu::MasterTable *getFicAlbaranVentaDet() const {
        return pFicAlbaranVentaDet;
    }
    factu::MasterTable *getFicFacturaVenta() const {
        return pFicFacturaVenta;
    }
    factu::MasterTable *getFicFacturaVentaDet() const {
        return pFicFacturaVentaDet;
    }
    factu::MasterTable *getFicPedidoCompra() const {
        return pFicPedidoCompra;
    }
    factu::MasterTable *getFicPedidoCompraDet() const {
        return pFicPedidoCompraDet;
    }
    factu::MasterTable *getFicAlbaranCompra() const {
        return pFicAlbaranCompra;
    }
    factu::MasterTable *getFicAlbaranCompraDet() const {
        return pFicAlbaranCompraDet;
    }
    factu::MasterTable *getFicFacturaCompra() const {
        return pFicFacturaCompra;
    }
    factu::MasterTable *getFicFacturaCompraDet() const {
        return pFicFacturaCompraDet;
    }
    NamesListTable *pFicTipoCliente;
    NamesListTable *pFicEstadoPedido;

private:
    factu::MasterTable *pFicArticulo;
    factu::MasterTable *pFicCliente;
    factu::MasterTable *pFicProveedora;
    factu::MasterTable *pFicAgente;
    factu::MasterTable *pFicFamilia;
    factu::MasterTable *pFicTipoDoc;
    factu::MasterTable *pFicPresupuestoVenta;
    factu::MasterTable *pFicPresupuestoVentaDet;
    factu::MasterTable *pFicPedidoVenta;
    factu::MasterTable *pFicPedidoVentaDet;
    factu::MasterTable *pFicAlbaranVenta;
    factu::MasterTable *pFicAlbaranVentaDet;
    factu::MasterTable *pFicFacturaVenta;
    factu::MasterTable *pFicFacturaVentaDet;
    factu::MasterTable *pFicPedidoCompra;
    factu::MasterTable *pFicPedidoCompraDet;
    factu::MasterTable *pFicAlbaranCompra;
    factu::MasterTable *pFicAlbaranCompraDet;
    factu::MasterTable *pFicFacturaCompra;
    factu::MasterTable *pFicFacturaCompraDet;
    /*>>>>>FACTUMODULE_RECORD_DEFINITIONS*/
    factu::MasterTable *pFicArticuloImagen;
    Date mWorkingDate;
    XtringList mInsertables;

private slots:
    void slotMenuFactuCambiosMasivosArticulos();
    void slotMenuFactuBalanceCliPro();
    void slotMenuFactuEstadCompraVenta();
    void slotMenuFactuMod303();
    void slotMenuFactuGenPedidosCompras();
    void slotMenuFactuRegenAsientos();
    void slotMenuFactuCopiarDocumento();
    /*<<<<<FACTUMODULE_MENU_DEFINITIONS*/
private slots:
    void slotMenuFacturacionArticulo();
    void slotMenuFacturacionCliente();
    void slotMenuFacturacionProveedora();
    void slotMenuFacturacionAgente();
    void slotMenuFacturacionFamilia();
    void slotMenuFacturacionTipoDoc();
    void slotMenuVentasPresupuestoVenta();
    void slotMenuVentasPedidoVenta();
    void slotMenuVentasAlbaranVenta();
    void slotMenuVentasFacturaVenta();
    void slotMenuComprasPedidoCompra();
    void slotMenuComprasAlbaranCompra();
    void slotMenuComprasFacturaCompra();
protected:
    QAction *pMenuFacturacionArticulo;
    QAction *pMenuFacturacionCliente;
    QAction *pMenuFacturacionProveedora;
    QAction *pMenuFacturacionAgente;
    QAction *pMenuFacturacionFamilia;
    QAction *pMenuFacturacionTipoDoc;
    QAction *pMenuVentasPresupuestoVenta;
    QAction *pMenuVentasPedidoVenta;
    QAction *pMenuVentasAlbaranVenta;
    QAction *pMenuVentasFacturaVenta;
    QAction *pMenuComprasPedidoCompra;
    QAction *pMenuComprasAlbaranCompra;
    QAction *pMenuComprasFacturaCompra;
    /*>>>>>FACTUMODULE_MENU_DEFINITIONS*/
    QMenu *pMenuFacturacion, *pMenuCompras, *pMenuVentas, *pMenuInformes, *pMenuOperaciones;
    QAction *pMenuFactuRegenAsientos, *pMenuFactuCopiarDocumento;
    QAction *pMenuFactuEstadCompraVenta, *pMenuFactuGenPedidosCompras, *pMenuFactuBalanceCliPro;
    QAction *pMenuFactuCambiosMasivosArticulos, *pMenuFactuMod303;
public:
    QMenu *getMenuVentas() const {
        return pMenuVentas;
    }
    QMenu *getMenuCompras() const {
        return pMenuCompras;
    }
    QAction *getMenuFactuGenPedidosCompras() const {
        return pMenuFactuGenPedidosCompras;
    }
    QAction *getMenuFactuEstadCompraVenta() const {
        return pMenuFactuEstadCompraVenta;
    }
    QAction *getMenuFactuBalanceCliPro() const {
        return pMenuFactuBalanceCliPro;
    }
    QAction *getMenuFactuMod303() const {
        return pMenuFactuMod303;
    }

    /*<<<<<FACTUMODULE_REQUIRES*/
public:
    empresa::EmpresaModule *getEmpresaModule() const {
        return pEmpresaModule;
    }
    pagos::PagosModule *getPagosModule() const {
        return pPagosModule;
    }
#ifdef HAVE_CONTABMODULE
    contab::ContabModule *getContabModule() const {
        return pContabModule;
    }
#endif
private:
    empresa::EmpresaModule *pEmpresaModule;
    pagos::PagosModule *pPagosModule;
#ifdef HAVE_CONTABMODULE
    contab::ContabModule *pContabModule;
#endif
    /*>>>>>FACTUMODULE_REQUIRES*/
};

extern FactuModule *ModuleInstance;

/*<<<<<FACTUMODULE_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTUMODULE_H
/*>>>>>FACTUMODULE_POSTAMBLE*/

