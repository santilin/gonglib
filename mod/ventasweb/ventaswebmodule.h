/*<<<<<COPYLEFT*/
/** @file ventaswebmodule.h Module ventasweb
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
/*<<<<<VENTASWEBMODULE_PREAMBLE*/
#ifndef _VENTASWEB_VENTASWEBMODULE_H
#define _VENTASWEB_VENTASWEBMODULE_H
/*>>>>>VENTASWEBMODULE_PREAMBLE*/

/*<<<<<VENTASWEBMODULE_INCLUDES*/
#include "config.h"
#include <qobject.h>
#include <dbappdbmodule.h>
#include "ventaswebmastertable.h"

class QAction;
class QMenu;
#ifdef HAVE_EMPRESAMODULE
#include <empresamodule.h>
#else
#error El módulo 'ventasweb::VentasWebModule' requiere el módulo 'Empresa'
#endif
#ifdef HAVE_FACTUMODULE
#include <factumodule.h>
#else
#error El módulo 'ventasweb::VentasWebModule' requiere el módulo 'Factu'
#endif
/*>>>>>VENTASWEBMODULE_INCLUDES*/

namespace gong {

#ifdef HAVE_SOCIASMODULE
namespace socias {
class RecMiembro;
}
#else
namespace factu {
class RecCliente;
}
#endif

namespace ventasweb {

/*<<<<<VENTASWEBMODULE_CLASS_DEFINITION*/
class VentasWebModule: public QObject, public dbModule
{
	Q_OBJECT // Si da error de funciones virtuales no encontradas, elimina esto
public:
	VentasWebModule(int argc, char *argv[]);
	virtual ~VentasWebModule();
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
/*>>>>>VENTASWEBMODULE_CLASS_DEFINITION*/
    virtual void afterCreateEditForm( FrmEditRec *frm, dbRecord *rec ); // from dbModule
    virtual Xtring getMigrationSQL( uint old_version ) const;

    int getMaxContador(const Xtring& table, dbRecordID empresa_id) const;
#ifdef HAVE_SOCIASMODULE
    int createJoomlaUser( socias::RecMiembro *miembro, const Xtring &server, const Xtring &filename  ) const;
#else
    int createJoomlaUser( factu::RecCliente *cliente, const Xtring &server, const Xtring &filename ) const;
#endif

    /*<<<<<VENTASWEBMODULE_RECORD_DEFINITIONS*/
public:
	ventasweb::MasterTable *getFicPedidoWeb() const { return pFicPedidoWeb; }
	ventasweb::MasterTable *getFicPedidoWebDet() const { return pFicPedidoWebDet; }
	ventasweb::MasterTable *getFicSemanaPedido() const { return pFicSemanaPedido; }

private:
	ventasweb::MasterTable *pFicPedidoWeb;
	ventasweb::MasterTable *pFicPedidoWebDet;
	ventasweb::MasterTable *pFicSemanaPedido;
/*>>>>>VENTASWEBMODULE_RECORD_DEFINITIONS*/

protected slots:
    void slotMenuVentasWebSubir();
    void slotMenuVentasWebBajar();
    void slotMenuVentasWebCambiosMasivosArticulos();
    void slotMenuVentasWebCreateJoomlaUser();

    /*<<<<<VENTASWEBMODULE_MENU_DEFINITIONS*/
private slots:
	void slotMenuVentasWebPedidoWeb();
	void slotMenuVentasWebSemanaPedido();
protected:
	QAction *pMenuVentasWebPedidoWeb;
	QAction *pMenuVentasWebSemanaPedido;
/*>>>>>VENTASWEBMODULE_MENU_DEFINITIONS*/
    QMenu *pMenuVentasWeb;
    QAction *pMenuVentasWebSubir, *pMenuVentasWebBajar, *pMenuVentasWebCreateJoomlaUser;

    /*<<<<<VENTASWEBMODULE_REQUIRES*/
public:
	empresa::EmpresaModule *getEmpresaModule() const { return pEmpresaModule; }
	factu::FactuModule *getFactuModule() const { return pFactuModule; }
private:
	empresa::EmpresaModule *pEmpresaModule;
	factu::FactuModule *pFactuModule;
/*>>>>>VENTASWEBMODULE_REQUIRES*/
};

extern VentasWebModule *ModuleInstance;

/*<<<<<VENTASWEBMODULE_POSTAMBLE*/
} // namespace ventasweb
} // namespace gong
#endif // VENTASWEB_VENTASWEBMODULE_H
/*>>>>>VENTASWEBMODULE_POSTAMBLE*/

