/*<<<<<MODULE_INFO*/
// COPYLEFT Module Tpv
// RECORD SesionTPV FrmEditRecMaster TPV
// MODULE_REQUIRED Factu
// TYPE GongModule tpv::TpvModule
/*>>>>>MODULE_INFO*/

/*<<<<<TPVMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tpvmodule.h"
#include "tpvfrmeditsesiontpv.h"
/*>>>>>TPVMODULE_INCLUDES*/
#include <dbappreport.h>
#include <factumastertable.h>
#include "tpvfrmeditfamiliabehavior.h"
#include "tpvfrmeditarticulobehavior.h"
#include "tpvfrmeditagentebehavior.h"
#include "tpvfrmtpvscreen.h"
#include "tpvfrmestadcompraventa.h"

namespace gong {
namespace tpv {

TpvModule *ModuleInstance = 0;

static dbModuleSetting _settings[] = {
	{
		dbModuleSetting::Int,
		"NUMERO_TPV",
		_("Número de TPV de este ordenador"),
		"1",
		dbModuleSetting::Local
	},
	{
		dbModuleSetting::String,
		"MODALIDAD",
		_("Modalidad (BAR|TIENDA|CAJON|BALANZA|SOCIAS|FAVORITOS)"),
		"BAR",
		dbModuleSetting::All
	},
	{
		dbModuleSetting::Bool,
		"NOMBRES_MAYUSCULAS",
		_("Mostrar nombres en mayúsculas"),
		"false",
		dbModuleSetting::All
	},
	{
		dbModuleSetting::Int,
		"COLUMNAS_FAMILIAS",
		_("Número de columnas de familias"),
		"8",
		dbModuleSetting::Local
	},
	{
		dbModuleSetting::Int,
		"FILAS_FAMILIAS",
		_("Número de filas de familias"),
		"2",
		dbModuleSetting::Local
	},
	{
		dbModuleSetting::Int,
		"COLUMNAS_ARTICULOS",
		_("Número de columnas de artículos"),
		"8",
		dbModuleSetting::Local
	},
	{
		dbModuleSetting::Int,
		"FILAS_ARTICULOS",
		_("Número de filas de artículos"),
		"5",
		dbModuleSetting::Local
	},
	{
		dbModuleSetting::Int,
		"ANCHO_IMAGENES",
		_("Ancho de las imágenes"),
		"80",
		dbModuleSetting::Local
	},
	{
		dbModuleSetting::Int,
		"ALTO_IMAGENES",
		_("Alto de las imágenes"),
		"80",
		dbModuleSetting::Local
	},
	{
		dbModuleSetting::Int,
		"CODIGO_CLIENTE",
		_("Código del cliente por omisión para el TPV"),
		"1",
		dbModuleSetting::All
	},
	{
		dbModuleSetting::Int,
		"CODIGO_AGENTE",
		_("Código del agente por omisión para el TPV"),
		"1",
		dbModuleSetting::All
	},
	{
		dbModuleSetting::Int,
		"CODIGO_TIPODOC",
		_("Código del tipo de documento por omisión para el TPV"),
		"1",
		dbModuleSetting::All
	},
	{
		dbModuleSetting::Int,
		"CODIGO_FORMAPAGO",
		_("Código de la forma de pago por omisión para el TPV"),
		"1",
		dbModuleSetting::All
	},
#ifdef HAVE_SOCIASMODULE
	{
		dbModuleSetting::Int,
		"CODIGO_PROYECTO",
		_("Código del proyecto por omisión para las socias del TPV"),
		"1",
		dbModuleSetting::All
	},
#endif
	{
		dbModuleSetting::String,
		"TICKET_RTK",
		_("Plantilla del informe para los tickets"),
		"TICKET.rtk",
		dbModuleSetting::All
	},
	{
		dbModuleSetting::String,
		"COMMAND.OPENDRAWER",
		_("Comando para abrir el cajón"),
		"", // "echo \"\" | lp"
		dbModuleSetting::Local
	},
	{
		dbModuleSetting::String,
		"RS232.PUERTO",
		_("Nombre del puerto serie de la balanza"),
		"/dev/ttyS5",
		dbModuleSetting::Local
	},
	{dbModuleSetting::None}
};


TpvModule::TpvModule(int argc, char *argv[])
	: dbModule("tpv", argc, argv)
{
	ModuleInstance = this;
    _GONG_DEBUG_TRACE(1);
	pModuleSettings = _settings;
/*<<<<<TPVMODULE_PUBLIC_INFO*/
	mModuleRequires << "factu";
	mMasterTables << "SESIONTPV";
//	mDetailTables
	pFactuModule = static_cast< factu::FactuModule * >(DBAPP->findModule( "Factu" ));
/*>>>>>TPVMODULE_PUBLIC_INFO*/
}

TpvModule::~TpvModule()
{
	_GONG_DEBUG_TRACE(1);
}

void TpvModule::readSettings()
{
	dbModule::readSettings();
	setNumeroTPV( getModuleSetting( "NUMERO_TPV" ).toInt() );
}

bool TpvModule::initDatabase(dbDefinition *db)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT( db );
	pMainDatabase = db;

	factu::MasterTable *fmt = new factu::MasterTable( db->findTableDefinition( "FAMILIA" ) );
	fmt->addFieldBool("PARATPV")->setDefaultValue("0");
    fmt->addFieldAggregateRelation( "FAMILIAIMAGEN_ID", "ARTICULOIMAGEN.ID", true );
	fmt->addFieldInt("ORDENTPV")->setDefaultValue("0");
	delete fmt;
	factu::MasterTable *agmt = new factu::MasterTable( db->findTableDefinition( "AGENTE" ) );
	agmt->addFieldBool("PARATPV")->setDefaultValue("0");
	delete agmt;
	factu::MasterTable *amt = new factu::MasterTable( db->findTableDefinition( "ARTICULO" ) );
	amt->addFieldBool("PARATPV")->setDefaultValue("0");
	amt->addFieldEuro("PVPTPV")->setDefaultValue("0.0");
	amt->addFieldString("VARIANTE1", 100);
	amt->addFieldEuro("PVPVARIANTE1")->setDefaultValue("0.0");
	amt->addFieldString("VARIANTE2", 100);
	amt->addFieldEuro("PVPVARIANTE2")->setDefaultValue("0.0");
	amt->addFieldInt("ORDENTPV")->setDefaultValue("0");
	delete amt;
	factu::MasterTable *avmt = new factu::MasterTable( db->findTableDefinition( "ALBARANVENTA" ) );
	avmt->addFieldTime("HORA");
	avmt->addFieldString("MESA", 50);
	avmt->addFieldOne2OneRelation( "SESIONTPV_ID", "SESIONTPV.ID", true );
	delete avmt;

	pFicSesionTPV = new factu::MasterTable( *pMainDatabase, "SESIONTPV" );
    pFicSesionTPV->addFieldRecordID();
    pFicSesionTPV->addFieldEmpresaID();
    pFicSesionTPV->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicSesionTPV->addFieldCodigo()->setUnique( false );
	pFicSesionTPV->addFieldInt( "NUMEROTPV" )->setCanBeNull( false );
	pFicSesionTPV->addFieldDateTime( "INICIO" )->setCanBeNull( false );
	pFicSesionTPV->addFieldDateTime( "FIN" )->setCanBeNull( true );
	pFicSesionTPV->addFieldString( "MESASSVG", 40 )->setCanBeNull( false );
	pFicSesionTPV->addFieldReferenceID( "CLIENTE_ID", "CLIENTE.ID");
	pFicSesionTPV->addFieldEuro( "CAJAINICIAL" );
	pFicSesionTPV->addFieldText( "ESTADOMESAS" );
    pFicSesionTPV->addFieldNotas();
    pFicSesionTPV->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicSesionTPV->addMultipleIndex( "sesiontpv_codigo_empresa_ejercicio", "EMPRESA_ID,EJERCICIO,CODIGO", true );
    pMainDatabase->addTable( pFicSesionTPV->getTableDefinition() );

	return true;
}

bool TpvModule::login(const Xtring& version, Xtring& addTitle, bool startingapp)
{
	return true;
}


dbRecord *TpvModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
/*<<<<<TPVMODULE_CREATE_RECORD*/
	if( tablename.upper() == "SESIONTPV" )
		return new RecSesionTPV(getConnection(), recid, user);
/*>>>>>TPVMODULE_CREATE_RECORD*/
	return 0;
}
FrmEditRec *TpvModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<TPVMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "SESIONTPV" )
		return new FrmEditSesionTPV(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>TPVMODULE_CREATE_EDITFORM*/
	return 0;
}

void TpvModule::afterCreateEditForm(FrmEditRec* frm, dbRecord* rec)
{
    Xtring tablename = rec->getTableName();
    if ( tablename.upper() == "FAMILIA" )
		frm->addBehavior( new FrmEditFamiliaBehavior( frm ) );
    else if ( tablename.upper() == "ARTICULO" )
		frm->addBehavior( new FrmEditArticuloBehavior( frm ) );
    else if ( tablename.upper() == "AGENTE" )
		frm->addBehavior( new FrmEditAgenteBehavior( frm ) );
}

FrmEditRecDetail *TpvModule::createEditDetailForm(
	FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<TPVMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>TPVMODULE_CREATE_EDITFORM_DETAIL*/
	return 0;
}

bool TpvModule::initMainWindow(MainWindow *mainwin)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT(mainwin);
	pMainWindow = mainwin;
	pMenuTPV = new QMenu( pMainWindow );
	pMenuTPV->setObjectName( "MenuTpv" );
    pMainWindow->menuBar()->insertItem( "&TPV", pMenuTPV );
	pMenuTPVIniciaTpv = new QAction( toGUI(_("Iniciar TPV")), pMainWindow );
	pMainWindow->connect(pMenuTPVIniciaTpv, SIGNAL(activated()), this, SLOT(slotMenuTPVIniciaTpv()));
	pMenuTPVIniciaTpv->addTo(pMenuTPV);
/*<<<<<TPVMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("SESIONTPV")->getDescPlural();
		pMenuTPVSesionTPV = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuTPVSesionTPV->setObjectName( "MenuTPVSesionTPV" );
		pMenuTPVSesionTPV->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuTPVSesionTPV->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuTPVSesionTPV, SIGNAL(activated()), this, SLOT(slotMenuTPVSesionTPV()));
		pMenuTPVSesionTPV->addTo(pMenuTPV);
	}
/*>>>>>TPVMODULE_INITMAINWINDOW_MENUS*/
	pMenuTPVArqueoCaja = new QAction( toGUI(_("Arqueo de caja")), pMainWindow );
	pMenuTPVArqueoCaja->setVisible( false );
	pMainWindow->connect(pMenuTPVArqueoCaja, SIGNAL(activated()), this, SLOT(slotMenuTPVArqueoCaja()));
	pMenuTPVArqueoCaja->addTo(pMenuTPV);
	pMenuTPVZActual = new QAction( toGUI(_("Imprimir X")), pMainWindow );
	pMainWindow->connect(pMenuTPVZActual , SIGNAL(activated()), this, SLOT(slotMenuTPVZActual()));
	pMenuTPVZActual->addTo(pMenuTPV);
	pMenuTPVCerrarSesion = new QAction( toGUI(_("Cerrar sesión actual")), pMainWindow );
	pMainWindow->connect(pMenuTPVCerrarSesion, SIGNAL(activated()), this, SLOT(slotMenuTPVCerrarSesion()));
	pMenuTPVCerrarSesion->addTo(pMenuTPV);
	return true;
}

/*<<<<<TPVMODULE_SLOT_TPVSESIONTPV*/
void TpvModule::slotMenuTPVSesionTPV()
{
	pMainWindow->slotMenuEditRecMaestro( "SESIONTPV" );
}
/*>>>>>TPVMODULE_SLOT_TPVSESIONTPV*/

FrmTpvScreen* TpvModule::initTPV()
{
	RecSesionTPV *sesion = RecSesionTPV::getCurrentSesion(
		ModuleInstance->getConnection(), getNumeroTPV() );
	dbRecordID sesiontpv_id = sesion->getRecordID();
	if( sesiontpv_id == 0 ) {
		DBAPP->showOSD( ("No hay sesiones definidas para ahora mismo"), _("Define la sesión ahora") );
		if( !DBAPP->editRecord( 0, sesion, 0, DataTable::inserting,
						   dbApplication::EditFlags( dbApplication::simpleEdition ) ) ) {
			delete sesion;
			return 0;
		}
		sesiontpv_id = sesion->getRecordID();
	}
	if( sesiontpv_id != 0 ) {
		Xtring conf_modalidad = ModuleInstance->getModuleSetting( "MODALIDAD" ).toString().upper();
		XtringList modalidades;
		conf_modalidad.tokenize( modalidades, "|" );
		int flags = FrmTpvScreen::ModNada;
		if( modalidades.contains("BAR") )
			flags += FrmTpvScreen::ModBar + FrmTpvScreen::ModRondas;
		if( modalidades.contains("TIENDA") )
			flags += FrmTpvScreen::ModTienda;
		if( modalidades.contains("BALANZA") )
			flags += FrmTpvScreen::ModBalanza;
		if( modalidades.contains("FAVORITOS") )
			flags += FrmTpvScreen::ModFilaFavoritos;
		if( modalidades.contains("CAJON") )
			flags += FrmTpvScreen::ModCajon;
#ifdef HAVE_SOCIASMODULE
		if( modalidades.contains("SOCIAS") )
			flags += FrmTpvScreen::ModSocias;
#endif
		return new FrmTpvScreen( static_cast<FrmTpvScreen::ModalidadTPV>(flags), sesion );
	}
	return 0;
}


void TpvModule::slotMenuTPVIniciaTpv()
{	
	FrmTpvScreen *frm = initTPV();
	DBAPP->createClient( frm );
}

void TpvModule::slotMenuTPVArqueoCaja()
{
	FrmEstadCompraVenta *frmstats = new FrmEstadCompraVenta();
	frmstats->show();
	pMainWindow->createClient ( frmstats );
}

void TpvModule::slotMenuTPVCerrarSesion()
{
	RecSesionTPV *sesion = RecSesionTPV::getCurrentSesion(
		ModuleInstance->getConnection(), getNumeroTPV() );
	dbRecordID sesiontpv_id = sesion->getRecordID();
	if( sesiontpv_id == 0 ) {
		FrmBase::msgError( _("TPV"), _("No hay sesiones abiertas ahora mismo") );
	} else {
		sesion->setValue( "FIN", DateTime::currentDateTime() );
		if( sesion->save(true, false ) ) {
			DBAPP->showOSD( _("TPV"), _("La sesión se ha cerrado correctamente") );
			if( FrmBase::msgYesNo( _("TPV"), _("¿Quieres imprimir la Z?") ) ) {
				printZ( sesiontpv_id );
			}
		} else {
			FrmBase::msgError( _("TPV"), _("No se ha podido cerrar la sesión") );
		}
	}
}

void TpvModule::slotMenuTPVZActual()
{
	RecSesionTPV *sesion = RecSesionTPV::getCurrentSesion(
		ModuleInstance->getConnection(), getNumeroTPV() );
	if( !sesion->getRecordID() ) {
		DBAPP->showOSD( ("No hay sesiones definidas para ahora mismo"), _("Elige la sesión ahora") );
		if( !DBAPP->choose( 0, sesion ) )
			return;
	}
	printZ( sesion->getRecordID() );
	delete sesion;
}


void TpvModule::printZ(dbRecordID sesiontpv_id)
{
	AppReport *report = new AppReport( *DBAPP, ModuleInstance->getConnection() );
	dbRecord *albven = DBAPP->createRecord( "ALBARANVENTA" );
	Xtring alb_filter = albven->getFilter("");
	delete albven;
	Xtring sesion_filter = "SESIONTPV_ID=" + getConnection()->toSQL( sesiontpv_id );
	if( !alb_filter.isEmpty() )
		sesion_filter += " AND " + alb_filter;
	Variant cobrado, resto;
	getConnection()->selectValues(
		"SELECT SUM(ENTREGA), SUM(RESTO) FROM ALBARANVENTA WHERE " + sesion_filter,
		&cobrado, &resto );
	Dictionary<Variant> properties;
	properties.insert( "COBRADO", cobrado ); 
	properties.insert( "RESTO", resto );
	report->readAndPrint( RTK_Screen, "TPVZ.rtk", sesion_filter,
						  Xtring::null, // init_defines
					      properties );
}


/*<<<<<TPVMODULE_FIN*/
} // namespace tpv
} // namespace gong

/*>>>>>TPVMODULE_FIN*/
/*<<<<<TPVMODULE_INIT_DATABASE*/

/*>>>>>TPVMODULE_INIT_DATABASE*/
