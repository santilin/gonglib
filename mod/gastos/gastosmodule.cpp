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
// RECORD GastoDef FrmEditRecMaster Gastos
// MODULE_OPTIONAL Pagos
// MODULE_OPTIONAL Contab
// MODULE_OPTIONAL Factu
// NAMESLISTTABLE PedirCampo
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
#include "gastosfrmeditgastodef.h"
/*>>>>>GASTOSMODULE_INCLUDES*/

namespace gong {
namespace gastos {

GastosModule *ModuleInstance = 0;

static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::String,
        "TIPODOC.GASTO",
        _("Tipo de documento de los gastos"),
        "0",
		dbModuleSetting::All
    },
    {
        dbModuleSetting::String,
        "TIPODOC.INGRESO",
        _("Tipo de documento de los ingresos"),
        "0",
		dbModuleSetting::All
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
//	mModuleRequires
	mMasterTables << "GASTODEF" << "PEDIRCAMPO" << "TIPOGASTO" << "CATEGORIAGASTO";
//	mDetailTables
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
	pFicPedirCampo = new NamesListTable( *pMainDatabase, "PEDIRCAMPO" );
	pMainDatabase->addTable( pFicPedirCampo->getTableDefinition() );
	pFicTipoGasto = new NamesListTable( *pMainDatabase, "TIPOGASTO" );
	pMainDatabase->addTable( pFicTipoGasto->getTableDefinition() );
	pFicCategoriaGasto = new NamesListTable( *pMainDatabase, "CATEGORIAGASTO" );
	pMainDatabase->addTable( pFicCategoriaGasto->getTableDefinition() );
/*>>>>>GASTOSMODULE_INIT_DATABASE*/

    pFicGastoDef = new MasterTable( *pMainDatabase, "GASTODEF" );
    pFicGastoDef->addFieldRecordID();
	pFicGastoDef->addField<FldNamesListTable>("TIPOGASTO");
	pFicGastoDef->addField<FldNamesListTable>("CATEGORIAGASTO");
	pFicGastoDef->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRIMPORTE" );
	pFicGastoDef->addFieldEuro( "IMPORTE" );
	pFicGastoDef->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRTERCERO" );
#ifdef HAVE_FACTUMODULE
	pFicGastoDef->addFieldOne2OneRelation( "CLIENTE_ID", "CLIENTE.ID", true );
	pFicGastoDef->addFieldOne2OneRelation( "PROVEEDORA_ID", "PROVEEDORA.ID", true );
#elif defined( HAVE_CONTABMODULE )
	pFicGastoDef->addFieldOne2OneRelation( "CUENTATERCERO_ID", "CUENTA.ID", true );
#else
	pFicGastoDef->addFieldOne2OneRelation( "CONTACTO_ID", "CONTACTO.ID", true );
#endif
	pFicGastoDef->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRCONCEPTO" );
#ifdef HAVE_FACTUMODULE
	pFicGastoDef->addFieldOne2OneRelation( "ARTICULO_ID", "ARTICULO.ID", false );
#elif defined( HAVE_CONTABMODULE )
	pFicGastoDef->addFieldOne2OneRelation( "CUENTACONCEPTO_ID", "CUENTA.ID", false );
#else
	pFicGastoDef->addFieldString( "CONCEPTO", 100 );
#endif
	pFicGastoDef->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRDESCRIPCION" );
	pFicGastoDef->addFieldText( "DESCRIPCION" );
	pFicGastoDef->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRPAGOS" );
    pFicGastoDef->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
#ifdef HAVE_CONTABMODULE
	pFicGastoDef->addFieldReferenceID( "CUENTAPAGO_ID", "CUENTA.ID" );
#else
	pFicGastoDef->addFieldString( "CUENTAPAGO", 50 );
#endif
	pFicGastoDef->addFieldString( "DOCUMENTOPAGO", 100 );
	pFicGastoDef->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRNOTAS" );
	pFicGastoDef->addFieldNotas();
	pFicGastoDef->addBehavior( DBAPP->getRecordTimestampBehavior() );
	pMainDatabase->addTable( pFicGastoDef->getTableDefinition() );
	return true;
}


dbRecord *GastosModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
/*<<<<<GASTOSMODULE_CREATE_RECORD*/
	if( tablename.upper() == "GASTODEF" )
		return new RecGastoDef(getConnection(), recid, user);
	if( tablename.upper() == "PEDIRCAMPO" )
		return new RecNamesListTable("PEDIRCAMPO", getConnection(), recid, user);
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
	if( tablename.upper() == "GASTODEF" )
		return new FrmEditGastoDef(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PEDIRCAMPO" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
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

/*<<<<<GASTOSMODULE_SLOT_GASTOSGASTODEF*/
void GastosModule::slotMenuGastosGastoDef()
{
	pMainWindow->slotMenuEditRecMaestro( "GASTODEF" );
}
/*>>>>>GASTOSMODULE_SLOT_GASTOSGASTODEF*/


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
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("GASTODEF")->getDescPlural();
		pMenuGastosGastoDef = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuGastosGastoDef->setObjectName( "MenuGastosGastoDef" );
		pMenuGastosGastoDef->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuGastosGastoDef->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuGastosGastoDef, SIGNAL(activated()), this, SLOT(slotMenuGastosGastoDef()));
		pMenuGastosGastoDef->addTo(pMenuGastos);
	}
/*>>>>>GASTOSMODULE_INITMAINWINDOW_MENUS*/
	return true;
}
/*<<<<<GASTOSMODULE_FIN*/
} // namespace gastos
} // namespace gong

/*>>>>>GASTOSMODULE_FIN*/
