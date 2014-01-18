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
// RECORD GastoTipo FrmEditRecMaster GastosTipo
// MODULE_OPTIONAL Pagos
// MODULE_OPTIONAL Contab
// MODULE_OPTIONAL Factu
// NAMESLISTTABLE PedirCampo
// NAMESLISTTABLE CategoriaGasto
// TYPE GongModule gastostipo::GastosTipoModule
/*>>>>>MODULE_INFO*/
/*<<<<<GASTOSTIPOMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include <dbappnameslisttable.h>
#include <dbappfrmeditnameslisttable.h>
#include "gastostipomodule.h"
#include "gastostipofrmeditgastotipo.h"
/*>>>>>GASTOSTIPOMODULE_INCLUDES*/

namespace gong {
namespace gastostipo {

GastosTipoModule *ModuleInstance = 0;

GastosTipoModule::GastosTipoModule()
	: dbModule("gastostipo")
{
	ModuleInstance = this;
    _GONG_DEBUG_TRACE(1);
/*<<<<<GASTOSTIPOMODULE_PUBLIC_INFO*/
//	mModuleRequires
	mMasterTables << "GASTOTIPO" << "PEDIRCAMPO" << "CATEGORIAGASTO";
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
/*>>>>>GASTOSTIPOMODULE_PUBLIC_INFO*/
}


GastosTipoModule::~GastosTipoModule()
{
	_GONG_DEBUG_TRACE(1);
}


bool GastosTipoModule::login(FrmLogin* frmlogin, const Xtring& version,
						 Xtring& addTitle, bool startingapp)
{
	return true;
}


bool GastosTipoModule::initDatabase(dbDefinition *db)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT( db );
	pMainDatabase = db;

/*<<<<<GASTOSTIPOMODULE_INIT_DATABASE*/
	pFicPedirCampo = new NamesListTable( *pMainDatabase, "PEDIRCAMPO" );
	pMainDatabase->addTable( pFicPedirCampo->getTableDefinition() );
	pFicCategoriaGasto = new NamesListTable( *pMainDatabase, "CATEGORIAGASTO" );
	pMainDatabase->addTable( pFicCategoriaGasto->getTableDefinition() );
/*>>>>>GASTOSTIPOMODULE_INIT_DATABASE*/

    pFicGastoTipo = new MasterTable( *pMainDatabase, "GASTOTIPO" );
    pFicGastoTipo->addFieldRecordID();
	pFicGastoTipo->addFieldDesc( "NOMBRE" );
	pFicGastoTipo->addFieldBool( "ESGASTO" )->setDefaultValue("1");
	pFicGastoTipo->addFieldBool( "REVISAR")->setDefaultValue("1");
	XtringList tables;
	tables << "ALBARANVENTA" << "ALBARANCOMPRA" << "FACTURAVENTA" << "FACTURACOMPRA" << "ASIENTO";
	XtringList captions;
	for( XtringList::const_iterator it = tables.begin(); it != tables.end(); ++ it )
		captions << DBAPP->getDatabase()->findTableDefinition( *it )->getName();
	pFicGastoTipo->addFieldListOfValues<Xtring>( false, captions, tables, "TABLENAME" );
	pFicGastoTipo->addField<FldNamesListTable>("CATEGORIAGASTO");
	pFicGastoTipo->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRIMPORTE" );
	pFicGastoTipo->addFieldEuro( "IMPORTE" );
	pFicGastoTipo->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRTERCERO" );
#ifdef HAVE_FACTUMODULE
	pFicGastoTipo->addFieldOne2OneRelation( "CLIENTE_ID", "CLIENTE.ID", true );
	pFicGastoTipo->addFieldOne2OneRelation( "PROVEEDORA_ID", "PROVEEDORA.ID", true );
#elif defined( HAVE_CONTABMODULE )
	pFicGastoTipo->addFieldOne2OneRelation( "CUENTATERCERO_ID", "CUENTA.ID", true );
#else
	pFicGastoTipo->addFieldOne2OneRelation( "CONTACTO_ID", "CONTACTO.ID", true );
#endif
	pFicGastoTipo->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRCONCEPTO" );
#ifdef HAVE_FACTUMODULE
	pFicGastoTipo->addFieldOne2OneRelation( "ARTICULO_ID", "ARTICULO.ID", false );
#elif defined( HAVE_CONTABMODULE )
	pFicGastoTipo->addFieldOne2OneRelation( "CUENTACONCEPTO_ID", "CUENTA.ID", false );
#else
	pFicGastoTipo->addFieldString( "CONCEPTO", 100 );
#endif
	pFicGastoTipo->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRDESCRIPCION" );
	pFicGastoTipo->addFieldText( "DESCRIPCION" );
	pFicGastoTipo->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRPAGOS" );
    pFicGastoTipo->addFieldOne2OneRelation( "FORMAPAGO_ID", "FORMAPAGO.ID", true );
#ifdef HAVE_CONTABMODULE
	pFicGastoTipo->addFieldReferenceID( "CUENTAPAGO_ID", "CUENTA.ID" );
#else
	pFicGastoTipo->addFieldString( "CUENTAPAGO", 50 );
#endif
	pFicGastoTipo->addFieldString( "DOCUMENTOPAGO", 100 );
	pFicGastoTipo->addField<FldNamesListTable>( Variant("PEDIRCAMPO"), "PEDIRNOTAS" );
	pFicGastoTipo->addFieldNotas();
	pFicGastoTipo->addBehavior( DBAPP->getRecordTimestampBehavior() );
	pMainDatabase->addTable( pFicGastoTipo->getTableDefinition() );
	return true;
}


dbRecord *GastosTipoModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
/*<<<<<GASTOSTIPOMODULE_CREATE_RECORD*/
	if( tablename.upper() == "GASTOTIPO" )
		return new RecGastoTipo(getConnection(), recid, user);
	if( tablename.upper() == "PEDIRCAMPO" )
		return new RecNamesListTable("PEDIRCAMPO", getConnection(), recid, user);
	if( tablename.upper() == "CATEGORIAGASTO" )
		return new RecNamesListTable("CATEGORIAGASTO", getConnection(), recid, user);
/*>>>>>GASTOSTIPOMODULE_CREATE_RECORD*/
	return 0;
}

FrmEditRec *GastosTipoModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<GASTOSTIPOMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "GASTOTIPO" )
		return new FrmEditGastoTipo(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PEDIRCAMPO" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "CATEGORIAGASTO" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>GASTOSTIPOMODULE_CREATE_EDITFORM*/
	return 0;
}

FrmEditRecDetail *GastosTipoModule::createEditDetailForm(
	FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<GASTOSTIPOMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>GASTOSTIPOMODULE_CREATE_EDITFORM_DETAIL*/
	return 0;
}


/*<<<<<GASTOSTIPOMODULE_SLOT_GASTOSTIPOGASTOTIPOGASTOTIPO*/
void GastosTipoModule::slotMenuGastosTipoGastoTipoGastoTipo()
{
	pMainWindow->slotMenuEditRecMaestro( "GASTOTIPO" );
}
/*>>>>>GASTOSTIPOMODULE_SLOT_GASTOSTIPOGASTOTIPOGASTOTIPO*/

bool GastosTipoModule::initMainWindow(MainWindow *mainwin)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT(mainwin);
	pMainWindow = mainwin;

	pMenuGastosTipo = 0;
#ifdef HAVE_FACTUMODULE
	if( getFactuModule() )
		pMenuGastosTipo = pMainWindow->findMenu( "MenuFacturacion" );
#elif defined( HAVE_CONTABMODULE )
	if( getContabModule() )
		pMenuGastosTipo = pMainWindow->findMenu( "MenuContabilidad" );
#endif
	if( !pMenuGastosTipo ) {
		pMenuGastosTipo = new QMenu( pMainWindow );
		pMenuGastosTipo->setObjectName( "MenuGastosTipo" );
		pMainWindow->menuBar()->insertItem( "&" + toGUI( DBAPP->getTableDescSingular("GASTOTIPO", "").c_str() ),
												pMenuGastosTipo );
	}

/*<<<<<GASTOSTIPOMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("GASTOTIPO")->getDescPlural();
		pMenuGastosTipoGastoTipoGastoTipo = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuGastosTipoGastoTipoGastoTipo->setObjectName( "MenuGastosTipoGastoTipoGastoTipo" );
		pMenuGastosTipoGastoTipoGastoTipo->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuGastosTipoGastoTipoGastoTipo->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuGastosTipoGastoTipoGastoTipo, SIGNAL(activated()), this, SLOT(slotMenuGastosTipoGastoTipoGastoTipo()));
		pMenuGastosTipoGastoTipoGastoTipo->addTo(pMenuGastosTipo);
	}
/*>>>>>GASTOSTIPOMODULE_INITMAINWINDOW_MENUS*/
	return true;
}
/*<<<<<GASTOSTIPOMODULE_FIN*/
} // namespace gastostipo
} // namespace gong

/*>>>>>GASTOSTIPOMODULE_FIN*/

