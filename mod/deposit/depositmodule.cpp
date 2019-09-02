/*<<<<<MODULE_INFO*/
// COPYLEFT Module deposit
// RECORD LiquidacionVenta FrmEditRecMaster Depositos
// RECORD LiquidacionVentaDet FrmEditRecDetail
// RECORD LiquidacionCompra FrmEditRecMaster Depositos
// RECORD LiquidacionCompraDet FrmEditRecDetail
// MODULE_REQUIRED Empresa
// MODULE_REQUIRED Factu
// MODULE_OPTIONAL Tesoreria
// TYPE GongModule deposit::DepositModule deposit
/*>>>>>MODULE_INFO*/

/*<<<<<DEPOSITMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "depositmodule.h"
#include "depositfrmeditliquidacionventa.h"
#include "depositfrmeditliquidacionventadet.h"
#include "depositfrmeditliquidacioncompra.h"
#include "depositfrmeditliquidacioncompradet.h"
/*>>>>>DEPOSITMODULE_INCLUDES*/
#include <factumodule.h>
#include "depositfrmfactliqventa.h"
#include "depositfrmgenliqcompra.h"
#include "depositfrmestadcompraventa.h"
#include "depositfrmedittipodocbehavior.h"
#include "depositfrmeditventadetbehavior.h"
#include "depositfrmliquidardepositos.h"

namespace gong {
namespace deposit {

DepositModule *ModuleInstance = 0;

DepositModule::DepositModule()
	: dbModule("deposit")
{
	ModuleInstance = this;
	_GONG_DEBUG_TRACE(1);
    pEmpresaModule = static_cast<empresa::EmpresaModule *>( DBAPP->findModule( "empresa" ) );
    _GONG_DEBUG_ASSERT( pEmpresaModule );
	pFactuModule = static_cast<factu::FactuModule *>( DBAPP->findModule( "factu" ) );
    _GONG_DEBUG_ASSERT( pFactuModule );
/*<<<<<DEPOSITMODULE_PUBLIC_INFO*/
	mModuleRequires << "empresa" << "factu";
	mMasterTables << "LIQUIDACIONVENTA" << "LIQUIDACIONCOMPRA";
	mDetailTables << "LIQUIDACIONVENTADET" << "LIQUIDACIONCOMPRADET";
	pEmpresaModule = static_cast< empresa::EmpresaModule * >(DBAPP->findModule( "Empresa" ));
	pFactuModule = static_cast< factu::FactuModule * >(DBAPP->findModule( "Factu" ));
#ifdef HAVE_TESORERIAMODULE
	pTesoreriaModule = static_cast< tesoreria::TesoreriaModule * >(DBAPP->findModule( "Tesoreria" ));
#endif
/*>>>>>DEPOSITMODULE_PUBLIC_INFO*/
	empresa::ModuleInstance->addContadorTable( "LIQUIDACIONVENTA" );
	factu::ModuleInstance->addInsertable("LIQUIDACIONVENTA");
	factu::ModuleInstance->addInsertable("LIQUIDACIONCOMPRA");
#ifdef HAVE_TESORERIAMODULE
	if( pTesoreriaModule && pTesoreriaModule->isEnabled() ) {
		pTesoreriaModule->getTablasDocumentos() << "LIQUIDACIONVENTA" << "LIQUIDACIONCOMPRA";
	}
#endif
}

DepositModule::~DepositModule()
{
	_GONG_DEBUG_TRACE(1);
}

bool DepositModule::login(FrmLogin *frmlogin, const Xtring& version,
						  Xtring& addTitle, bool startingapp)
{
	return true;
}


bool DepositModule::initDatabase(dbDefinition *db)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT( db );
	pMainDatabase = db;

	factu::MasterTable *fmt = new factu::MasterTable( db->findTableDefinition( "TIPODOC" ) );
    fmt->addFieldBool( "ESDEPOSITO" )->setDefaultValue( "0" );
    fmt->addFieldBool( "ESLIQUIDACION" )->setDefaultValue( "0" );
    delete fmt;

	fmt = new factu::MasterTable( db->findTableDefinition( "ALBARANVENTADET" ) );
    fmt->addFieldEuro4Dec( "IMPORTELIQUIDACION" );
    fmt->addFieldEuro( "IMPORTELIQUIDACIONIVA" );
	fmt->addFieldReferenceID( "LIQUIDACIONCOMPRA_ID", "LIQUIDACIONCOMPRA.ID" );
    delete fmt;

	fmt = new factu::MasterTable( db->findTableDefinition( "FACTURAVENTADET" ) );
    fmt->addFieldEuro4Dec( "IMPORTELIQUIDACION" );
    fmt->addFieldEuro( "IMPORTELIQUIDACIONIVA" );
	fmt->addFieldReferenceID( "LIQUIDACIONCOMPRA_ID", "LIQUIDACIONCOMPRA.ID" );
    delete fmt;

    pFicLiquidacionVenta = new deposit::MasterTable( *pMainDatabase, "LIQUIDACIONVENTA" );
    pFicLiquidacionVenta->addFieldRecordID();
    pFicLiquidacionVenta->addFieldEmpresaID();
    pFicLiquidacionVenta->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicLiquidacionVenta->addFieldOne2OneRelation( "TIPODOC_ID", "TIPODOC.ID", false );
	pFicLiquidacionVenta->addFieldInt( "CONTADOR" );
    pFicLiquidacionVenta->addFieldStringCode( "NUMERO", 20 )->setUnique(true);
    pFicLiquidacionVenta->addFieldFecha();
    pFicLiquidacionVenta->addFieldOne2OneRelation( "CLIENTE_ID", "CLIENTE.ID", false );
    pFicLiquidacionVenta->addFieldOne2OneRelation( "AGENTE_ID", "AGENTE.ID", true );
    pFicLiquidacionVenta->addField<factu::FldIVADetallado>( "IVADETALLADO" )->setCanBeNull( false );
    pFicLiquidacionVenta->addFieldsTotalesMaster( false, false ); // sin entrega a cuentas ni otros pagos
    pFicLiquidacionVenta->addFieldNotas();
    pFicLiquidacionVenta->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicLiquidacionVenta->addRelationDefinition( dbRelationDefinition::one2many, "LIQUIDACIONVENTA", "ID", "LIQUIDACIONVENTADET", "LIQUIDACIONVENTA_ID" );
	pFicLiquidacionVenta->addMultipleIndex( "liquidacionventa_cliente_numero_unico", "EMPRESA_ID,EJERCICIO,CLIENTE_ID,NUMERO", true );
    pMainDatabase->addTable( pFicLiquidacionVenta->getTableDefinition() );

    pFicLiquidacionVentaDet = new deposit::MasterTable( *pMainDatabase, "LIQUIDACIONVENTADET" );
    pFicLiquidacionVentaDet->addFieldRecordID();
    pFicLiquidacionVentaDet->addFieldReferenceID( "LIQUIDACIONVENTA_ID", "LIQUIDACIONVENTA.ID" );
    pFicLiquidacionVentaDet->addFieldsComunesDetalles( false /*ventas*/ );
    pFicLiquidacionVentaDet->addFieldEuro4Dec( "IMPORTELIQUIDACION" );
    pFicLiquidacionVentaDet->addFieldEuro( "IMPORTELIQUIDACIONIVA" );
    pFicLiquidacionVentaDet->addFieldNotas();
    pMainDatabase->addTable( pFicLiquidacionVentaDet->getTableDefinition() );

	pFicLiquidacionCompra = new deposit::MasterTable( *pMainDatabase, "LIQUIDACIONCOMPRA" );
    pFicLiquidacionCompra->addFieldRecordID();
    pFicLiquidacionCompra->addFieldEmpresaID();
    pFicLiquidacionCompra->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicLiquidacionCompra->addFieldOne2OneRelation( "TIPODOC_ID", "TIPODOC.ID", false );
	pFicLiquidacionCompra->addFieldInt( "CONTADOR" );
    pFicLiquidacionCompra->addFieldStringCode( "NUMERO", 20 )->setUnique(false);
    pFicLiquidacionCompra->addFieldFecha();
    pFicLiquidacionCompra->addFieldOne2OneRelation( "PROVEEDORA_ID", "PROVEEDORA.ID", false );
    pFicLiquidacionCompra->addField<factu::FldIVADetallado>( "IVADETALLADO" )->setCanBeNull( false );
    pFicLiquidacionCompra->addFieldsTotalesMaster( false, false ); // sin entrega a cuentas ni otros pagos
    pFicLiquidacionCompra->addFieldNotas();
    pFicLiquidacionCompra->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicLiquidacionCompra->addRelationDefinition( dbRelationDefinition::one2many, "LIQUIDACIONCOMPRA", "ID", "LIQUIDACIONCOMPRADET", "LIQUIDACIONCOMPRA_ID" );
	pFicLiquidacionCompra->addMultipleIndex( "liquidacioncompra_numero_unico", "EMPRESA_ID,EJERCICIO,PROVEEDORA_ID,NUMERO", true );
    pMainDatabase->addTable( pFicLiquidacionCompra->getTableDefinition() );

    pFicLiquidacionCompraDet = new deposit::MasterTable( *pMainDatabase, "LIQUIDACIONCOMPRADET" );
    pFicLiquidacionCompraDet->addFieldRecordID();
    pFicLiquidacionCompraDet->addFieldReferenceID( "LIQUIDACIONCOMPRA_ID", "LIQUIDACIONCOMPRA.ID" );
    pFicLiquidacionCompraDet->addFieldsComunesDetalles( true /*compras*/ );
    pFicLiquidacionCompraDet->addFieldNotas();
    pMainDatabase->addTable( pFicLiquidacionCompraDet->getTableDefinition() );

	return true;
}

dbRecord *DepositModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
/*<<<<<DEPOSITMODULE_CREATE_RECORD*/
	if( tablename.upper() == "LIQUIDACIONVENTA" )
		return new RecLiquidacionVenta(getConnection(), recid, user);
	if( tablename.upper() == "LIQUIDACIONVENTADET" )
		return new RecLiquidacionVentaDet(getConnection(), recid, user);
	if( tablename.upper() == "LIQUIDACIONCOMPRA" )
		return new RecLiquidacionCompra(getConnection(), recid, user);
	if( tablename.upper() == "LIQUIDACIONCOMPRADET" )
		return new RecLiquidacionCompraDet(getConnection(), recid, user);
/*>>>>>DEPOSITMODULE_CREATE_RECORD*/
	return 0;
}

FrmEditRec *DepositModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<DEPOSITMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "LIQUIDACIONVENTA" )
		return new FrmEditLiquidacionVenta(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "LIQUIDACIONCOMPRA" )
		return new FrmEditLiquidacionCompra(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>DEPOSITMODULE_CREATE_EDITFORM*/
	return 0;
}

void DepositModule::afterCreateEditForm( FrmEditRec *frm, dbRecord *rec )
{
	if( rec->getTableName() == "TIPODOC" ) {
		frm->addBehavior( new FrmEditTipoDocBehavior( frm ) );
	}
	if( rec->getTableName() == "ALBARANVENTADET" ) {
		frm->addBehavior( new FrmEditVentaDetBehavior( frm ) );
	}
	if( rec->getTableName() == "FACTURAVENTADET" ) {
		frm->addBehavior( new FrmEditVentaDetBehavior( frm ) );
	}
}

FrmEditRecDetail *DepositModule::createEditDetailForm(
	FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<DEPOSITMODULE_CREATE_EDITFORM_DETAIL*/
	if( tablename.upper() == "LIQUIDACIONVENTADET" )
		return new FrmEditLiquidacionVentaDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "LIQUIDACIONCOMPRADET" )
		return new FrmEditLiquidacionCompraDet(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
/*>>>>>DEPOSITMODULE_CREATE_EDITFORM_DETAIL*/
	return 0;
}

bool DepositModule::initMainWindow(MainWindow *mainwin)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT(mainwin);
	pMainWindow = mainwin;
	QMenu *pMenuFacturacion = factu::ModuleInstance->getMenuFacturacion();

	pMenuDepositos = new QMenu( toGUI( _("Depósitos") ), pMainWindow );
	pMenuDepositos->setObjectName( "MenuDepositos" );
	pMenuFacturacion->addMenu( pMenuDepositos );

/*<<<<<DEPOSITMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("LIQUIDACIONVENTA")->getDescPlural();
		pMenuDepositosLiquidacionVenta = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuDepositosLiquidacionVenta->setObjectName( "MenuDepositosLiquidacionVenta" );
		pMenuDepositosLiquidacionVenta->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuDepositosLiquidacionVenta->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuDepositosLiquidacionVenta, SIGNAL(activated()), this, SLOT(slotMenuDepositosLiquidacionVenta()));
		pMenuDepositosLiquidacionVenta->addTo(pMenuDepositos);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("LIQUIDACIONCOMPRA")->getDescPlural();
		pMenuDepositosLiquidacionCompra = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuDepositosLiquidacionCompra->setObjectName( "MenuDepositosLiquidacionCompra" );
		pMenuDepositosLiquidacionCompra->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuDepositosLiquidacionCompra->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuDepositosLiquidacionCompra, SIGNAL(activated()), this, SLOT(slotMenuDepositosLiquidacionCompra()));
		pMenuDepositosLiquidacionCompra->addTo(pMenuDepositos);
	}
/*>>>>>DEPOSITMODULE_INITMAINWINDOW_MENUS*/
    pMenuLiquidacionDepositosVenta = new QAction( toGUI( _( "Generar liquidaciones de clientes" ) ), 0, pMainWindow, 0 );
    pMainWindow->connect( pMenuLiquidacionDepositosVenta, SIGNAL( activated() ), this, SLOT( slotMenuLiquidacionDepositosVenta() ) );
    pMenuLiquidacionDepositosVenta->addTo( pMenuDepositos );

    pMenuLiquidacionDepositosCompra = new QAction( toGUI( _( "Generar liquidaciones a proveedoras" ) ), 0, pMainWindow, 0 );
    pMainWindow->connect( pMenuLiquidacionDepositosCompra, SIGNAL( activated() ), this, SLOT( slotMenuLiquidacionDepositosCompra() ) );
    pMenuLiquidacionDepositosCompra->addTo( pMenuDepositos );

	pMenuLiquidacionVenta = new QAction( toGUI( _( "Facturar liquidaciones de clientes" ) ), 0, pMainWindow, 0 );
    pMainWindow->connect( pMenuLiquidacionVenta, SIGNAL( activated() ), this, SLOT( slotMenuLiquidacionFacturarLiquidacionesVenta() ) );
    pMenuLiquidacionVenta->addTo( pMenuDepositos );

	pMainWindow->disconnect( pFactuModule->getMenuFactuEstadCompraVenta(), SIGNAL( activated() ), 0, 0 );
    pMainWindow->connect( pFactuModule->getMenuFactuEstadCompraVenta(),
						  SIGNAL( activated() ), this,
						  SLOT( slotMenuLiquidacionInformeDepositos() ) );
	return true;
}

/*<<<<<DEPOSITMODULE_SLOT_DEPOSITOSLIQUIDACIONVENTA*/
void DepositModule::slotMenuDepositosLiquidacionVenta()
{
	pMainWindow->slotMenuEditRecMaestro( "LIQUIDACIONVENTA" );
}
/*>>>>>DEPOSITMODULE_SLOT_DEPOSITOSLIQUIDACIONVENTA*/
/*<<<<<DEPOSITMODULE_SLOT_DEPOSITOSLIQUIDACIONCOMPRA*/
void DepositModule::slotMenuDepositosLiquidacionCompra()
{
	pMainWindow->slotMenuEditRecMaestro( "LIQUIDACIONCOMPRA" );
}
/*>>>>>DEPOSITMODULE_SLOT_DEPOSITOSLIQUIDACIONCOMPRA*/

void DepositModule::slotMenuLiquidacionInformeDepositos()
{
	FrmEstadCompraVenta *frmstats = new FrmEstadCompraVenta();
	frmstats->show();
	DBAPP->createClient ( frmstats );
}

void DepositModule::slotMenuLiquidacionFacturarLiquidacionesVenta()
{
	if( DBAPP->preventOperationIf( DBAPP->isReadOnly(),
		"Facturar liquidaciones de clientes", "la base de datos está bloqueada" ) )
		return;
	FrmFactLiqVenta *frmfactliqventa = new FrmFactLiqVenta();
	frmfactliqventa->show();
	DBAPP->createClient( frmfactliqventa );
}

void DepositModule::slotMenuLiquidacionDepositosCompra()
{
	if( DBAPP->preventOperationIf( DBAPP->isReadOnly(),
		"Liquidar depósitos de proveedoras", "la base de datos está bloqueada" ) )
		return;
	FrmGenLiqCompra *frmgenliqcompra = new FrmGenLiqCompra();
	frmgenliqcompra->show();
	DBAPP->createClient( frmgenliqcompra );
}

void DepositModule::slotMenuLiquidacionDepositosVenta()
{
	if( DBAPP->preventOperationIf( DBAPP->isReadOnly(),
		"Liquidar depósitos a clientes", "la base de datos está bloqueada" ) )
		return;
	FrmLiquidarDepositos *frmliqdep = new FrmLiquidarDepositos( FrmLiquidarDepositos::ventas );
	frmliqdep->show();
	DBAPP->createClient( frmliqdep );
}


/*<<<<<DEPOSITMODULE_FIN*/
} // namespace deposit
} // namespace gong

/*>>>>>DEPOSITMODULE_FIN*/
/*<<<<<DEPOSITMODULE_INIT_DATABASE*/

/*>>>>>DEPOSITMODULE_INIT_DATABASE*/
