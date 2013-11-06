/*<<<<<COPYLEFT*/
/** @file gastosmodule.cpp Module gastos
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Module gastos
// RECORD Gasto FrmEditRecMaster Gastos
// MODULE_REQUIRED Gastos
// MODULE_OPTIONAL Pagos
// MODULE_OPTIONAL Contab
// MODULE_OPTIONAL Factu
// NAMESLISTTABLE TipoGasto
// NAMESLISTTABLE CategoriaGasto
// TYPE GongModule gastos::GastosModule
/*>>>>>MODULE_INFO*/
/*<<<<<GASTOSMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include <dbappnameslisttable.h>
#include <dbappfrmeditnameslisttable.h>
#include "gastosmodule.h"
#include "gastosfrmeditgasto.h"
/*>>>>>GASTOSMODULE_INCLUDES*/

namespace gong {
namespace gastos {

GastosModule *ModuleInstance = 0;

static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::String,
        "TIPODOC.GASTO",
        _("Tipo de documento de los gastos"),
        "0"
    },
    {
        dbModuleSetting::String,
        "TIPODOC.INGRESO",
        _("Tipo de documento de los ingresos"),
        "0"
    },
    {dbModuleSetting::None}
};


GastosModule::GastosModule()
	: dbModule("gastos")
{
	ModuleInstance = this;
    _GONG_DEBUG_TRACE(1);
	pModuleSettings = _settings;
/*<<<<<GASTOSMODULE_PUBLIC_INFO*/
	mModuleRequires << "gastos";
	mMasterTables << "GASTO" << "TIPOGASTO" << "CATEGORIAGASTO";
//	mDetailTables
	pGastosModule = static_cast< gastos::GastosModule * >(DBAPP->findModule( "Gastos" ));
#ifdef HAVE_PAGOSMODULE
	pPagosModule = static_cast< pagos::PagosModule * >(DBAPP->findModule( "Pagos" ));
#endif
#ifdef HAVE_CONTABMODULE
	pContabModule = static_cast< contab::ContabModule * >(DBAPP->findModule( "Contab" ));
#endif
#ifdef HAVE_FACTUMODULE
	pFactuModule = static_cast< factu::FactuModule * >(DBAPP->findModule( "Factu" ));
#endif
/*>>>>>GASTOSMODULE_PUBLIC_INFO*/
}


GastosModule::~GastosModule()
{
	_GONG_DEBUG_TRACE(1);
}


bool GastosModule::login(FrmLogin* frmlogin, const Xtring& version,
						 Xtring& addTitle, bool startingapp)
{
	return true;
}


bool GastosModule::initDatabase(dbDefinition *db)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT( db );
	pMainDatabase = db;

/*<<<<<GASTOSMODULE_INIT_DATABASE*/
	pFicTipoGasto = new NamesListTable( *pMainDatabase, "TIPOGASTO" );
	pMainDatabase->addTable( pFicTipoGasto->getTableDefinition() );
	pFicCategoriaGasto = new NamesListTable( *pMainDatabase, "CATEGORIAGASTO" );
	pMainDatabase->addTable( pFicCategoriaGasto->getTableDefinition() );
/*>>>>>GASTOSMODULE_INIT_DATABASE*/

    pFicGasto = new empresa::MasterTable( *pMainDatabase, "GASTO" );
    pFicGasto->addFieldRecordID();
    pFicGasto->addFieldEmpresaID();
    pFicGasto->addField<empresa::FldEjercicio>( "EJERCICIO" );
	pFicGasto->addField<FldNamesListTable>("TIPOGASTO");
	pFicGasto->addField<FldNamesListTable>("CATEGORIAGASTO");
	pFicGasto->addFieldInt( "NUMERO" );
	pFicGasto->addFieldDate( "FECHA" );
	pFicGasto->addFieldEuro( "IMPORTE");
#ifdef HAVE_FACTUMODULE
	pFicGasto->addFieldOne2OneRelation( "CLIENTE_ID", "CLIENTE.ID", true );
	pFicGasto->addFieldOne2OneRelation( "PROVEEDORA_ID", "PROVEEDORA.ID", true );
#elif defined( HAVE_CONTABMODULE )
	pFicGasto->addFieldOne2OneRelation( "CUENTATERCERO_ID", "CUENTA.ID", true );
#else
	pFicGasto->addFieldOne2OneRelation( "CONTACTO_ID", "CONTACTO.ID", true );
#endif
#ifdef HAVE_FACTUMODULE
	pFicGasto->addFieldOne2OneRelation( "ARTICULO_ID", "ARTICULO.ID", false );
#elif defined( HAVE_CONTABMODULE )
	pFicGasto->addFieldOne2OneRelation( "CUENTACONCEPTO_ID", "CUENTA.ID", false );
#else
	pFicGasto->addFieldString( "CONCEPTO", 100 );
#endif
	pFicGasto->addFieldText( "DESCRIPCION" );
#ifdef HAVE_CONTABMODULE
	pFicGasto->addFieldReferenceID( "CUENTAPAGO_ID", "CUENTA.ID" );
#else
	pFicGasto->addFieldString( "CUENTAPAGO", 50 );
#endif
	pFicGasto->addFieldString( "DOCUMENTOPAGO", 100 );
	pFicGasto->addFieldBool( "GENALBARAN" );
	pFicGasto->addFieldBool( "GENASIENTO" );
    pFicGasto->addFieldString( "NUMEROALBARAN", 30 );
    pFicGasto->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
	pFicGasto->addFieldNotas();
	pFicGasto->addBehavior( DBAPP->getRecordTimestampBehavior() );
	pMainDatabase->addTable( pFicGasto->getTableDefinition() );
	return true;
}


dbRecord *GastosModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
/*<<<<<GASTOSMODULE_CREATE_RECORD*/
	if( tablename.upper() == "GASTO" )
		return new RecGasto(getConnection(), recid, user);
	if( tablename.upper() == "TIPOGASTO" )
		return new RecNamesListTable("TIPOGASTO", getConnection(), recid, user);
	if( tablename.upper() == "CATEGORIAGASTO" )
		return new RecNamesListTable("CATEGORIAGASTO", getConnection(), recid, user);
/*>>>>>GASTOSMODULE_CREATE_RECORD*/
	return 0;
}

FrmEditRec *GastosModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<GASTOSMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "GASTO" )
		return new FrmEditGasto(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "TIPOGASTO" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "CATEGORIAGASTO" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>GASTOSMODULE_CREATE_EDITFORM*/
	return 0;
}

FrmEditRecDetail *GastosModule::createEditDetailForm(
	FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<GASTOSMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>GASTOSMODULE_CREATE_EDITFORM_DETAIL*/
	return 0;
}

/*<<<<<GASTOSMODULE_SLOT_GASTOSGASTO*/
void GastosModule::slotMenuGastosGasto()
{
	pMainWindow->slotMenuEditRecMaestro( "GASTO" );
}
/*>>>>>GASTOSMODULE_SLOT_GASTOSGASTO*/


bool GastosModule::initMainWindow(MainWindow *mainwin)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT(mainwin);
	pMainWindow = mainwin;

	pMenuGastos = 0;
#ifdef HAVE_FACTUMODULE
	if( getFactuModule() )
		pMenuGastos = pMainWindow->findMenu( "MenuFacturacion" );
#elif defined( HAVE_CONTABMODULE )
	if( getContabModule() )
		pMenuGastos = pMainWindow->findMenu( "MenuContabilidad" );
#endif
	if( !pMenuGastos ) {
		pMenuGastos = new QMenu( pMainWindow );
		pMenuGastos->setObjectName( "MenuGastos" );
		pMainWindow->menuBar()->insertItem( "&" + toGUI( DBAPP->getTableDescSingular("GASTOS", "").c_str() ),
												pMenuGastos );
	}

/*<<<<<GASTOSMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("GASTO")->getDescPlural();
		pMenuGastosGasto = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuGastosGasto->setObjectName( "MenuGastosGasto" );
		pMenuGastosGasto->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuGastosGasto->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuGastosGasto, SIGNAL(activated()), this, SLOT(slotMenuGastosGasto()));
		pMenuGastosGasto->addTo(pMenuGastos);
	}
/*>>>>>GASTOSMODULE_INITMAINWINDOW_MENUS*/
	return true;
}
/*<<<<<GASTOSMODULE_FIN*/
} // namespace gastos
} // namespace gong

/*>>>>>GASTOSMODULE_FIN*/
