/*<<<<<MODULE_INFO*/
// COPYLEFT Module tesoreria
// RECORD ApunteTesoreria FrmEditRecMaster Tesoreria
// RECORD CuentaTesoreria FrmEditRecMaster Tesoreria
// RECORD TerceroTesoreria FrmEditRecMaster Tesoreria
// RECORD ConceptoTesoreria FrmEditRecMaster Tesoreria
// RECORD TipoApunteTesoreria FrmEditRecMaster Tesoreria
// TYPE GongModule tesoreria::TesoreriaModule
/*>>>>>MODULE_INFO*/

/*<<<<<TESORERIAMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "tesoreriamodule.h"
#include "tesoreriafrmeditapuntetesoreria.h"
#include "tesoreriafrmeditcuentatesoreria.h"
#include "tesoreriafrmedittercerotesoreria.h"
#include "tesoreriafrmeditconceptotesoreria.h"
#include "tesoreriafrmedittipoapuntetesoreria.h"
/*>>>>>TESORERIAMODULE_INCLUDES*/
#include "tesoreriafrmeditempresabehavior.h"


namespace gong {
namespace tesoreria {

TesoreriaModule *ModuleInstance = 0;

static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::Bool,
        "SUPERVISAR_APUNTES",
        _("Supervisar los apuntes que se generan automáticamente"),
        "true",
        dbModuleSetting::All
	},
    {dbModuleSetting::None}
};


TesoreriaModule::TesoreriaModule()
    : dbModule("tesoreria")
{
    _GONG_DEBUG_TRACE(1);
    ModuleInstance = this;
    pModuleSettings = _settings;
/*<<<<<TESORERIAMODULE_PUBLIC_INFO*/
//	mModuleRequires
	mMasterTables << "APUNTETESORERIA" << "CUENTATESORERIA" << "TERCEROTESORERIA" << "CONCEPTOTESORERIA" << "TIPOAPUNTETESORERIA";
//	mDetailTables
/*>>>>>TESORERIAMODULE_PUBLIC_INFO*/
    mTablasConceptos << "CONCEPTOTESORERIA";
    mTablasTerceros << "CUENTATESORERIA" << "CONTACTO";
    mTablasCuentas << "CUENTATESORERIA";
}


TesoreriaModule::~TesoreriaModule()
{
    _GONG_DEBUG_TRACE(1);
}

bool TesoreriaModule::login(FrmLogin* frmlogin, const Xtring& version, Xtring& addTitle, bool startingapp)
{
    return true;
}


void TesoreriaModule::afterLoad()
{
    FldPedirCampo *fpc = new FldPedirCampo( "foo", "bar" );
    fpc->setValuesFromString( ModuleInstance->getModuleSetting( "PEDIRCAMPO.VALUES" ).toString() );
    delete fpc;
}


bool TesoreriaModule::initDatabase(dbDefinition *db)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;
/*<<<<<TESORERIAMODULE_INIT_DATABASE*/

/*>>>>>TESORERIAMODULE_INIT_DATABASE*/

    tesoreria::MasterTable *cmt = new tesoreria::MasterTable( db->findTableDefinition( "EMPRESA" ) );
    cmt->addFieldString( "CUENTACAJA", 14 );
    delete cmt;

    pFicTipoApunteTesoreria = new MasterTable( *pMainDatabase, "TIPOAPUNTETESORERIA" );
    pFicTipoApunteTesoreria->addFieldRecordID();
    pFicTipoApunteTesoreria->addFieldEmpresaID();
    pFicTipoApunteTesoreria->addFieldIntCode( "CODIGO" )->setUnique(false);
    pFicTipoApunteTesoreria->addFieldDesc( "NOMBRE", 50 );
    pFicTipoApunteTesoreria->addField<FldPedirCampo>( "PEDIRFECHA" );
    pFicTipoApunteTesoreria->addFieldString("FECHA",20);
    pFicTipoApunteTesoreria->addField<FldPedirCampo>( "PEDIRIMPORTE" );
    pFicTipoApunteTesoreria->addFieldMoney("IMPORTE");
    pFicTipoApunteTesoreria->addField<FldPedirCampo>( "PEDIRREFERENCIA" );
    pFicTipoApunteTesoreria->addFieldString("REFERENCIA",100);
    pFicTipoApunteTesoreria->addField<FldPedirCampo>( "PEDIRCUENTA" );
    pFicTipoApunteTesoreria->addFieldString("CUENTA",200);
    pFicTipoApunteTesoreria->addField<FldPedirCampo>( "PEDIRTABLATERCEROS" );
    pFicTipoApunteTesoreria->addFieldListOfValues<Xtring>( false, &getTablasTerceros(), &getTablasTerceros(),
            "TABLATERCEROS" )->setCanBeNull(true);
    pFicTipoApunteTesoreria->addField<FldPedirCampo>( "PEDIRTERCERO" );
    pFicTipoApunteTesoreria->addFieldString("TERCERO",200);
    pFicTipoApunteTesoreria->addField<FldPedirCampo>( "PEDIRTABLACONCEPTOS" );
    pFicTipoApunteTesoreria->addFieldListOfValues<Xtring>( false, &getTablasConceptos(), &getTablasConceptos(),
            "TABLACONCEPTOS" )->setCanBeNull(true);
    pFicTipoApunteTesoreria->addField<FldPedirCampo>( "PEDIRCONCEPTO" );
    pFicTipoApunteTesoreria->addFieldString("CONCEPTO",200);
    pFicTipoApunteTesoreria->addField<FldPedirCampo>( "PEDIRNOTAS" );
    pFicTipoApunteTesoreria->addFieldString("VALORNOTAS",200);
    pFicTipoApunteTesoreria->addField<FldPedirCampo>( "PEDIRPROYECTO" );
    pFicTipoApunteTesoreria->addFieldString("PROYECTO",200);
    pFicTipoApunteTesoreria->addFieldNotas("NOTAS");
    pFicTipoApunteTesoreria->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicTipoApunteTesoreria->getTableDefinition() );

    pFicTerceroTesoreria = new MasterTable( *pMainDatabase, "TERCEROTESORERIA" );
    pFicTerceroTesoreria->addFieldRecordID();
    pFicTerceroTesoreria->addFieldEmpresaID();
    pFicTerceroTesoreria->addFieldIntCode( "CODIGO" )->setUnique(false);
    pFicTerceroTesoreria->addFieldDesc( "NOMBRE", 150 );
    pFicTerceroTesoreria->addFieldNotas();
    pFicTerceroTesoreria->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicTerceroTesoreria->getTableDefinition() );

    pFicConceptoTesoreria = new MasterTable( *pMainDatabase, "CONCEPTOTESORERIA" );
    pFicConceptoTesoreria->addFieldRecordID();
    pFicConceptoTesoreria->addFieldEmpresaID();
    pFicConceptoTesoreria->addFieldIntCode( "CODIGO" )->setUnique(false);
    pFicConceptoTesoreria->addFieldDesc( "NOMBRE", 150 );
    pFicConceptoTesoreria->addFieldNotas();
    pFicConceptoTesoreria->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicConceptoTesoreria->getTableDefinition() );


    // Las cuentas son independientes para cada empresa y ejercicio
    pFicCuentaTesoreria = new MasterTable( *pMainDatabase, "CUENTATESORERIA" );
    pFicCuentaTesoreria->addFieldRecordID();
    pFicCuentaTesoreria->addFieldEmpresaID();
    pFicCuentaTesoreria->addFieldStringCode( "CODIGO", 20 )->setUnique(false);
    pFicCuentaTesoreria->addFieldDesc( "NOMBRE", 150 );
    pFicCuentaTesoreria->addFieldEuro( "SALDOINICIAL" );
    pFicCuentaTesoreria->addFieldDate( "FECHASALDOINICIAL" );
    pFicCuentaTesoreria->addFieldEuro( "SALDO" )->setReadOnly( true );
    pFicCuentaTesoreria->addFieldNotas();
    pFicCuentaTesoreria->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicCuentaTesoreria->addMultipleIndex( "cuenta_codigo_unico", "EMPRESA_ID,CUENTA", true );
    pMainDatabase->addTable( pFicCuentaTesoreria->getTableDefinition() );

    pFicApunteTesoreria = new MasterTable( *pMainDatabase, "APUNTETESORERIA" );
    pFicApunteTesoreria->addFieldRecordID();
    pFicApunteTesoreria->addFieldEmpresaID();
    pFicApunteTesoreria->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicApunteTesoreria->addFieldOne2OneRelation( "TIPOAPUNTETESORERIA_ID", "TIPOAPUNTETESORERIA.ID" );
    pFicApunteTesoreria->addFieldInt( "NUMERO" );
    pFicApunteTesoreria->addFieldDate( "FECHA" );
    pFicApunteTesoreria->addFieldEuro( "IMPORTE" );
    pFicApunteTesoreria->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID", true );
    pFicApunteTesoreria->addFieldOne2OneRelation( "CUENTATESORERIA_ID", "CUENTATESORERIA.ID" );
    pFicApunteTesoreria->addFieldListOfValues<Xtring>( false, &getTablasTerceros(), &getTablasTerceros(),
            "TABLATERCEROS" )->setCanBeNull(true);
    pFicApunteTesoreria->addFieldReferenceID( "TERCERO_ID", "TERCERO.ID" );
    pFicApunteTesoreria->addFieldDesc( "TERCERO", 200 )->setCanBeNull( true );
    pFicApunteTesoreria->addFieldListOfValues<Xtring>( false, &getTablasConceptos(), &getTablasConceptos(),
            "TABLACONCEPTOS" )->setCanBeNull(true);
    pFicApunteTesoreria->addFieldReferenceID( "CONCEPTO_ID", "CONCEPTO.ID" );
    pFicApunteTesoreria->addFieldDesc( "CONCEPTO", 200 )->setCanBeNull( true );
    pFicApunteTesoreria->addFieldListOfValues<Xtring>( false, &getTablasDocumentos(), &getTablasDocumentos(),
            "TABLADOCUMENTOS")->setCanBeNull(true);
    pFicApunteTesoreria->addFieldReferenceID( "DOCUMENTO_ID", "DOCUMENTO.ID" );
    pFicApunteTesoreria->addFieldDesc( "REFERENCIA", 200 )->setCanBeNull( true );
    pFicApunteTesoreria->addFieldBool( "AUTOMATICO" )->setDefaultValue( "0" )->setReadOnly( true );
    pFicApunteTesoreria->addFieldNotas();
    pFicApunteTesoreria->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicApunteTesoreria->getTableDefinition() );

    return true;
}


dbRecord *TesoreriaModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    /*<<<<<TESORERIAMODULE_CREATE_RECORD*/
	if( tablename.upper() == "APUNTETESORERIA" )
		return new RecApunteTesoreria(getConnection(), recid, user);
	if( tablename.upper() == "CUENTATESORERIA" )
		return new RecCuentaTesoreria(getConnection(), recid, user);
	if( tablename.upper() == "TERCEROTESORERIA" )
		return new RecTerceroTesoreria(getConnection(), recid, user);
	if( tablename.upper() == "CONCEPTOTESORERIA" )
		return new RecConceptoTesoreria(getConnection(), recid, user);
	if( tablename.upper() == "TIPOAPUNTETESORERIA" )
		return new RecTipoApunteTesoreria(getConnection(), recid, user);
/*>>>>>TESORERIAMODULE_CREATE_RECORD*/
    return 0;
}

FrmEditRec *TesoreriaModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<TESORERIAMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "APUNTETESORERIA" )
		return new FrmEditApunteTesoreria(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "CUENTATESORERIA" )
		return new FrmEditCuentaTesoreria(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "TERCEROTESORERIA" )
		return new FrmEditTerceroTesoreria(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "CONCEPTOTESORERIA" )
		return new FrmEditConceptoTesoreria(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "TIPOAPUNTETESORERIA" )
		return new FrmEditTipoApunteTesoreria(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>TESORERIAMODULE_CREATE_EDITFORM*/
    return 0;
}


void TesoreriaModule::afterCreateEditForm(FrmEditRec* frm, dbRecord* rec)
{
    if( rec->getTableName() == "EMPRESA" ) {
        frm->addBehavior( new FrmEditEmpresaBehavior( frm ) );
    }
}


FrmEditRecDetail *TesoreriaModule::createEditDetailForm(
    FrmEditRecMaster *frmmaster, int ndetail,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<TESORERIAMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>TESORERIAMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}

/*<<<<<TESORERIAMODULE_SLOT_TESORERIACUENTATESORERIA*/
void TesoreriaModule::slotMenuTesoreriaCuentaTesoreria()
{
	pMainWindow->slotMenuEditRecMaestro( "CUENTATESORERIA" );
}
/*>>>>>TESORERIAMODULE_SLOT_TESORERIACUENTATESORERIA*/
/*<<<<<TESORERIAMODULE_SLOT_TESORERIAAPUNTETESORERIA*/
void TesoreriaModule::slotMenuTesoreriaApunteTesoreria()
{
	pMainWindow->slotMenuEditRecMaestro( "APUNTETESORERIA" );
}
/*>>>>>TESORERIAMODULE_SLOT_TESORERIAAPUNTETESORERIA*/
/*<<<<<TESORERIAMODULE_SLOT_TESORERIATERCEROTESORERIA*/
void TesoreriaModule::slotMenuTesoreriaTerceroTesoreria()
{
	pMainWindow->slotMenuEditRecMaestro( "TERCEROTESORERIA" );
}
/*>>>>>TESORERIAMODULE_SLOT_TESORERIATERCEROTESORERIA*/
/*<<<<<TESORERIAMODULE_SLOT_TESORERIACONCEPTOTESORERIA*/
void TesoreriaModule::slotMenuTesoreriaConceptoTesoreria()
{
	pMainWindow->slotMenuEditRecMaestro( "CONCEPTOTESORERIA" );
}
/*>>>>>TESORERIAMODULE_SLOT_TESORERIACONCEPTOTESORERIA*/

/*<<<<<TESORERIAMODULE_SLOT_TESORERIATIPOAPUNTETESORERIA*/
void TesoreriaModule::slotMenuTesoreriaTipoApunteTesoreria()
{
	pMainWindow->slotMenuEditRecMaestro( "TIPOAPUNTETESORERIA" );
}
/*>>>>>TESORERIAMODULE_SLOT_TESORERIATIPOAPUNTETESORERIA*/

bool TesoreriaModule::initMainWindow(MainWindow *mainwin)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT(mainwin);
    pMainWindow = mainwin;

    pMenuTesoreria = new QMenu( pMainWindow );
    pMenuTesoreria->setObjectName( "MenuTesoreria" );
    pMainWindow->menuBar()->insertItem( toGUI( _( "&Tesorería" ) ), pMenuTesoreria );

    /*<<<<<TESORERIAMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("APUNTETESORERIA")->getDescPlural();
		pMenuTesoreriaApunteTesoreria = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuTesoreriaApunteTesoreria->setObjectName( "MenuTesoreriaApunteTesoreria" );
		pMenuTesoreriaApunteTesoreria->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuTesoreriaApunteTesoreria->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuTesoreriaApunteTesoreria, SIGNAL(activated()), this, SLOT(slotMenuTesoreriaApunteTesoreria()));
		pMenuTesoreriaApunteTesoreria->addTo(pMenuTesoreria);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("CUENTATESORERIA")->getDescPlural();
		pMenuTesoreriaCuentaTesoreria = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuTesoreriaCuentaTesoreria->setObjectName( "MenuTesoreriaCuentaTesoreria" );
		pMenuTesoreriaCuentaTesoreria->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuTesoreriaCuentaTesoreria->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuTesoreriaCuentaTesoreria, SIGNAL(activated()), this, SLOT(slotMenuTesoreriaCuentaTesoreria()));
		pMenuTesoreriaCuentaTesoreria->addTo(pMenuTesoreria);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("TERCEROTESORERIA")->getDescPlural();
		pMenuTesoreriaTerceroTesoreria = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuTesoreriaTerceroTesoreria->setObjectName( "MenuTesoreriaTerceroTesoreria" );
		pMenuTesoreriaTerceroTesoreria->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuTesoreriaTerceroTesoreria->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuTesoreriaTerceroTesoreria, SIGNAL(activated()), this, SLOT(slotMenuTesoreriaTerceroTesoreria()));
		pMenuTesoreriaTerceroTesoreria->addTo(pMenuTesoreria);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("CONCEPTOTESORERIA")->getDescPlural();
		pMenuTesoreriaConceptoTesoreria = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuTesoreriaConceptoTesoreria->setObjectName( "MenuTesoreriaConceptoTesoreria" );
		pMenuTesoreriaConceptoTesoreria->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuTesoreriaConceptoTesoreria->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuTesoreriaConceptoTesoreria, SIGNAL(activated()), this, SLOT(slotMenuTesoreriaConceptoTesoreria()));
		pMenuTesoreriaConceptoTesoreria->addTo(pMenuTesoreria);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("TIPOAPUNTETESORERIA")->getDescPlural();
		pMenuTesoreriaTipoApunteTesoreria = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuTesoreriaTipoApunteTesoreria->setObjectName( "MenuTesoreriaTipoApunteTesoreria" );
		pMenuTesoreriaTipoApunteTesoreria->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuTesoreriaTipoApunteTesoreria->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuTesoreriaTipoApunteTesoreria, SIGNAL(activated()), this, SLOT(slotMenuTesoreriaTipoApunteTesoreria()));
		pMenuTesoreriaTipoApunteTesoreria->addTo(pMenuTesoreria);
	}
/*>>>>>TESORERIAMODULE_INITMAINWINDOW_MENUS*/
    return true;
}


/*<<<<<TESORERIAMODULE_FIN*/
} // namespace tesoreria
} // namespace gong

/*>>>>>TESORERIAMODULE_FIN*/

