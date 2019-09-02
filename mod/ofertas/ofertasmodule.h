/*<<<<<COPYLEFT*/
/** @file ofertasmodule.h Module ofertas
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
/*<<<<<OFERTASMODULE_PREAMBLE*/
#ifndef _OFERTAS_OFERTASMODULE_H
#define _OFERTAS_OFERTASMODULE_H
/*>>>>>OFERTASMODULE_PREAMBLE*/

/*<<<<<OFERTASMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "ofertasmastertable.h"

class QAction;
class QMenu;
#ifdef HAVE_EMPRESAMODULE
#include <empresamodule.h>
#else
#error El módulo 'ofertas::OfertasModule' requiere el módulo 'Empresa'
#endif
#ifdef HAVE_FACTUMODULE
#include <factumodule.h>
#else
#error El módulo 'ofertas::OfertasModule' requiere el módulo 'Factu'
#endif
#ifdef HAVE_PAGOSMODULE
#include <pagosmodule.h>
#else
#error El módulo 'ofertas::OfertasModule' requiere el módulo 'Pagos'
#endif
#ifdef HAVE_TESORERIAMODULE
#include <tesoreriamodule.h>
#endif
/*>>>>>OFERTASMODULE_INCLUDES*/

namespace gong {
namespace ofertas {

class dbDoubleConnection;

/*<<<<<OFERTASMODULE_CLASS_DEFINITION*/
class OfertasModule: public QObject, public dbModule
{
	Q_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto
public:
	OfertasModule(int argc, char *argv[]);
	virtual ~OfertasModule();
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
/*>>>>>OFERTASMODULE_CLASS_DEFINITION*/
    virtual void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec ); // from dbModule
    int getMaxContador() const;
    int copiaCobros(dbRecord *origen, dbRecord *nueva);

    /*<<<<<OFERTASMODULE_RECORD_DEFINITIONS*/
public:
	ofertas::MasterTable *getFicOfertaCompra() const { return pFicOfertaCompra; }
	ofertas::MasterTable *getFicOfertaCompraDet() const { return pFicOfertaCompraDet; }
	ofertas::MasterTable *getFicOfertaVenta() const { return pFicOfertaVenta; }
	ofertas::MasterTable *getFicOfertaVentaDet() const { return pFicOfertaVentaDet; }
	ofertas::MasterTable *getFicPagoOferta() const { return pFicPagoOferta; }
	ofertas::MasterTable *getFicCobroOferta() const { return pFicCobroOferta; }
	ofertas::MasterTable *getFicApunteOferta() const { return pFicApunteOferta; }
	ofertas::MasterTable *getFicCuentaOferta() const { return pFicCuentaOferta; }

private:
	ofertas::MasterTable *pFicOfertaCompra;
	ofertas::MasterTable *pFicOfertaCompraDet;
	ofertas::MasterTable *pFicOfertaVenta;
	ofertas::MasterTable *pFicOfertaVentaDet;
	ofertas::MasterTable *pFicPagoOferta;
	ofertas::MasterTable *pFicCobroOferta;
	ofertas::MasterTable *pFicApunteOferta;
	ofertas::MasterTable *pFicCuentaOferta;
/*>>>>>OFERTASMODULE_RECORD_DEFINITIONS*/

    /*<<<<<OFERTASMODULE_REQUIRES*/
public:
	empresa::EmpresaModule *getEmpresaModule() const { return pEmpresaModule; }
	factu::FactuModule *getFactuModule() const { return pFactuModule; }
	pagos::PagosModule *getPagosModule() const { return pPagosModule; }
#ifdef HAVE_TESORERIAMODULE
	tesoreria::TesoreriaModule *getTesoreriaModule() const { return pTesoreriaModule; }
#endif
private:
	empresa::EmpresaModule *pEmpresaModule;
	factu::FactuModule *pFactuModule;
	pagos::PagosModule *pPagosModule;
#ifdef HAVE_TESORERIAMODULE
	tesoreria::TesoreriaModule *pTesoreriaModule;
#endif
/*>>>>>OFERTASMODULE_REQUIRES*/
private slots:
    void slotMenuOfertasCopiarAlbaranesVenta();
    void slotMenuOfertasCopiarAlbaranesCompra();
    void slotMenuOfertasCopiarFacturasVenta();
    void slotMenuOfertasCopiarFacturasCompra();

/*<<<<<OFERTASMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuOfertasOfertaCompra();
	void slotMenuOfertasOfertaVenta();
	void slotMenuOfertasPagoOferta();
	void slotMenuOfertasCobroOferta();
protected:
	QAction *pMenuOfertasOfertaCompra;
	QAction *pMenuOfertasOfertaVenta;
	QAction *pMenuOfertasPagoOferta;
	QAction *pMenuOfertasCobroOferta;
	QAction *pMenuOfertasApunteOferta;
	QAction *pMenuOfertasCuentaOferta;
/*>>>>>OFERTASMODULE_MENU_DEFINITIONS*/
//{capel} Poner slotMenuOfertasApunte y Cuenta entre #ifdef HAVE_TESORERIAMODULE
    QAction *pMenuOfertasCopiarAlbaranesVenta, *pMenuOfertasCopiarAlbaranesCompra;
    QAction *pMenuOfertasCopiarFacturasVenta, *pMenuOfertasCopiarFacturasCompra;
    QMenu *pMenuOfertas;
    dbDefinition *pReplicaDatabase;
    dbDoubleConnection *pDoubleConnection;
    Xtring mDBName;
    bool mActivo;
};

extern OfertasModule *ModuleInstance;
/*<<<<<OFERTASMODULE_POSTAMBLE*/
} // namespace ofertas
} // namespace gong
#endif // OFERTAS_OFERTASMODULE_H
/*>>>>>OFERTASMODULE_POSTAMBLE*/

