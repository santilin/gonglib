/*<<<<<MODULE_INFO*/
// COPYLEFT Module tesoreria
// RECORD CuentaTesoreria FrmEditRecMaster Tesoreria
// RECORD ApunteTesoreria FrmEditRecMaster Tesoreria
// NAMESLISTTABLE TipoApunteTesoreria
// TYPE GongModule tesoreria::TesoreriaModule
/*>>>>>MODULE_INFO*/

/*<<<<<TESORERIAMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include <dbappnameslisttable.h>
#include <dbappfrmeditnameslisttable.h>
#include "tesoreriamodule.h"
#include "tesoreriafrmeditcuentatesoreria.h"
#include "tesoreriafrmeditapuntetesoreria.h"
/*>>>>>TESORERIAMODULE_INCLUDES*/


namespace gong {
namespace tesoreria {

TesoreriaModule *ModuleInstance = 0;

TesoreriaModule::TesoreriaModule()
	: dbModule("tesoreria")
{
	ModuleInstance = this;
    _GONG_DEBUG_TRACE(1);
/*<<<<<TESORERIAMODULE_PUBLIC_INFO*/
//	mModuleRequires
	mMasterTables << "CUENTATESORERIA" << "APUNTETESORERIA" << "TIPOAPUNTETESORERIA";
//	mDetailTables
/*>>>>>TESORERIAMODULE_PUBLIC_INFO*/
}


TesoreriaModule::~TesoreriaModule()
{
	_GONG_DEBUG_TRACE(1);
}

bool TesoreriaModule::login(FrmLogin* frmlogin, const Xtring& version, Xtring& addTitle, bool startingapp)
{
	return true;
}


bool TesoreriaModule::initDatabase(dbDefinition *db)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT( db );
	pMainDatabase = db;
/*<<<<<TESORERIAMODULE_INIT_DATABASE*/
	pFicTipoApunteTesoreria = new NamesListTable( *pMainDatabase, "TIPOAPUNTETESORERIA" );
	pMainDatabase->addTable( pFicTipoApunteTesoreria->getTableDefinition() );
/*>>>>>TESORERIAMODULE_INIT_DATABASE*/

    // Las cuentas son independientes para cada empresa y ejercicio
    pFicCuentaTesoreria = new MasterTable( *pMainDatabase, "CUENTATESORERIA" );
    pFicCuentaTesoreria->addFieldRecordID();
    pFicCuentaTesoreria->addFieldEmpresaID();
    pFicCuentaTesoreria->addFieldStringCode( "CODIGO", 20 )->setUnique(false);
    pFicCuentaTesoreria->addFieldDesc( "NOMBRE", 150 );
    pFicCuentaTesoreria->addFieldEuro( "SALDO" )->setReadOnly( true );
    pFicCuentaTesoreria->addFieldEuro( "DEBE" )->setReadOnly( true );
    pFicCuentaTesoreria->addFieldEuro( "HABER" )->setReadOnly( true );
    pFicCuentaTesoreria->addFieldNotas();
    pFicCuentaTesoreria->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicCuentaTesoreria->addMultipleIndex( "cuenta_codigo_unico", "EMPRESA_ID,CUENTA", true );
    pMainDatabase->addTable( pFicCuentaTesoreria->getTableDefinition() );

    pFicApunteTesoreria = new MasterTable( *pMainDatabase, "APUNTETESORERIA" );
    pFicApunteTesoreria->addFieldRecordID();
    pFicApunteTesoreria->addFieldEmpresaID();
    pFicApunteTesoreria->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicApunteTesoreria->addFieldInt( "NUMERO" );
    pFicApunteTesoreria->addFieldDate( "FECHA" );
    pFicApunteTesoreria->addField<FldNamesListTable>( "TIPOAPUNTETESORERIA" );
    pFicApunteTesoreria->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID", true );
    pFicApunteTesoreria->addFieldOne2OneRelation( "CUENTATESORERIA_ID", "CUENTATESORERIA.ID" );
    pFicApunteTesoreria->addFieldEuro( "DEBE" );
    pFicApunteTesoreria->addFieldEuro( "HABER" );
    pFicApunteTesoreria->addFieldDesc( "CONCEPTO", 100 )->setCanBeNull( true );
    pFicApunteTesoreria->addFieldString( "TABLATERCEROS", 30 );
    pFicApunteTesoreria->addFieldReferenceID( "TERCERO_ID", "TERCERO.ID" );
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
	if( tablename.upper() == "CUENTATESORERIA" )
		return new RecCuentaTesoreria(getConnection(), recid, user);
	if( tablename.upper() == "APUNTETESORERIA" )
		return new RecApunteTesoreria(getConnection(), recid, user);
	if( tablename.upper() == "TIPOAPUNTETESORERIA" )
		return new RecNamesListTable("TIPOAPUNTETESORERIA", getConnection(), recid, user);
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
	if( tablename.upper() == "CUENTATESORERIA" )
		return new FrmEditCuentaTesoreria(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "APUNTETESORERIA" )
		return new FrmEditApunteTesoreria(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "TIPOAPUNTETESORERIA" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>TESORERIAMODULE_CREATE_EDITFORM*/
	return 0;
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
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("CUENTATESORERIA")->getDescPlural();
		pMenuTesoreriaCuentaTesoreria = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuTesoreriaCuentaTesoreria->setObjectName( "MenuTesoreriaCuentaTesoreria" );
		pMenuTesoreriaCuentaTesoreria->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuTesoreriaCuentaTesoreria->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuTesoreriaCuentaTesoreria, SIGNAL(activated()), this, SLOT(slotMenuTesoreriaCuentaTesoreria()));
		pMenuTesoreriaCuentaTesoreria->addTo(pMenuTesoreria);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("APUNTETESORERIA")->getDescPlural();
		pMenuTesoreriaApunteTesoreria = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuTesoreriaApunteTesoreria->setObjectName( "MenuTesoreriaApunteTesoreria" );
		pMenuTesoreriaApunteTesoreria->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuTesoreriaApunteTesoreria->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuTesoreriaApunteTesoreria, SIGNAL(activated()), this, SLOT(slotMenuTesoreriaApunteTesoreria()));
		pMenuTesoreriaApunteTesoreria->addTo(pMenuTesoreria);
	}
/*>>>>>TESORERIAMODULE_INITMAINWINDOW_MENUS*/
	return true;
}

#if 0
RecApunteTesoreria* TesoreriaModule::creaAsientoSimple(FrmEditRec* parent, 
		RecApunteTesoreria* old_apunte,
        Date fecha, const Xtring& rec_origen,
        dbRecordID proyecto_id, const Xtring& descripcion,
        const Xtring& concepto, dbRecordID cuentadebe_id, Money debe,
        dbRecordID cuentahaber_id, Money haber, bool supervisar)
{
    RecCuentaTesoreria *reccuenta = static_cast<RecCuentaTesoreria *>(DBAPP->createRecord("CUENTATESORERIA"));
    reccuenta->read( cuentadebe_id );
    Xtring cuentadebe = reccuenta->getValue("CODIGO").toString();
    reccuenta->read( cuentahaber_id );
    Xtring cuentahaber = reccuenta->getValue("CODIGO").toString();
    return creaAsientoSimple( parent, old_apunte, fecha, rec_origen, proyecto_id, descripcion,
                              concepto, cuentadebe, debe, cuentahaber, haber, supervisar );
}


RecApunteTesoreria *TesoreriaModule::creaAsientoSimple(FrmEditRec *parent, 
		RecApunteTesoreria *old_apunte,
        Date fecha, const Xtring &rec_origen, dbRecordID proyecto_id, 
		const Xtring &descripcion, const Xtring &concepto, 
		const Xtring &cuentadebe, Money debe,
        const Xtring &cuentahaber, Money haber,	bool supervisar)
{
    Date fechacontable = empresa::ModuleInstance->getRecEmpresa()->getValue("FECHACONTABLE").toDate();
    if( fechacontable.isValid() && fecha < fechacontable ) {
        DBAPP->showStickyOSD(_("Crear apunte"), _("No se ha creado el apunte porque la fecha es anterior a la fecha contable de esta empresa") );
        return 0;
    }
    DBAPP->waitCursor( true );
    RecApunteTesoreria *apunte = static_cast<RecApunteTesoreria *>( DBAPP->createRecord("APUNTETESORERIA") );
    apunte->clear( true );
    apunte->setValue( "AUTOMATICO", true );
    apunte->setValue( "FECHA", fecha );
    apunte->setValue( "PROYECTO_ID", proyecto_id );
    apunte->setValue( "DESCRIPCION", descripcion );
	apunte->setValue( "TIPOAPUNTE", 5 ); // Automático
    if( old_apunte ) {
        apunte->rescateValues( old_apunte );
        apunte->setValue( "ID", old_apunte->getRecordID() );
        old_apunte->remove();
    }
    contab::RecApunte *apuntedebe = static_cast<contab::RecApunte *>(DBAPP->createRecord( "APUNTETESORERIA" ));
    Cuenta cuentadebe_contraida(cuentadebe, getDigitosTrabajo());
    cuentadebe_contraida.contraer( getMaxNivelGrupo() );
    Cuenta cuentahaber_contraida(cuentahaber, getDigitosTrabajo());
    cuentahaber_contraida.contraer( getMaxNivelGrupo() );
    apuntedebe->quickValues( Cuenta(cuentadebe, getDigitosTrabajo()), Xtring::null, debe, 0,
                             concepto, cuentahaber_contraida );
    apunte->addApunte( apuntedebe );
    contab::RecApunte *apuntehaber = static_cast<contab::RecApunte *>(DBAPP->createRecord( "APUNTE" ));
    apuntehaber->quickValues( Cuenta(cuentahaber, getDigitosTrabajo()), Xtring::null, 0, haber,
                              concepto, cuentadebe_contraida );
    apunte->addApunte( apuntehaber );

    FrmEditRec *frmeditapunte = DBAPP->createEditForm(parent,
                                 apunte, 0, DataTable::inserting, dbApplication::simpleEdition, parent );
    if( !supervisar )
        supervisar = !frmeditapunte->showAndSave();
    if( supervisar )
        frmeditapunte->showModalFor( parent, true, true );
    if( frmeditapunte->isSaved() ) {
        DBAPP->getMainWindow()->refreshByName( Xtring::null,Xtring::null ); // TODO refresh only related
    }
    delete frmeditapunte;
    DBAPP->resetCursor();
    return apunte;
}

RecApunteTesoreria *TesoreriaModule::creaAsientoMultiple(FrmEditRec* parent, RecApunteTesoreria *old_apunte,
        Date fecha, const Xtring& rec_origen, dbRecordID proyecto_id,
        const Xtring &descripcion,
        List< ApunteQuickValues > apuntevalues, bool supervisar )
{
    Date fechacontable = empresa::ModuleInstance->getRecEmpresa()->getValue("FECHACONTABLE").toDate();
    if( fechacontable.isValid() && fecha < fechacontable ) {
        DBAPP->showStickyOSD(_("Crear apunte"), _("No se ha creado el apunte porque la fecha es anterior a la fecha contable de esta empresa") );
        return 0;
    }
    RecApunteTesoreria *apunte = static_cast<RecApunteTesoreria *>( DBAPP->createRecord("ASIENTO") );
    apunte->clear( true );
    apunte->setValue( "AUTOMATICO", true );
    apunte->setValue( "FECHA", fecha );
    apunte->setValue( "PROYECTO_ID", proyecto_id );
	apunte->setValue( "TIPOASIENTO", 5 ); // Automático
// 	int contador = apunte->getLastNumDocumento( proyecto_id ) + 1;
// 	apunte->setValue( "CONTADORNUMDOCUMENTO", contador );
// 	if( apunte->getRecProyecto()->read( proyecto_id ) ) {
// 		int codproyecto = apunte->getRecProyecto()->getValue( "CODIGO" ).toInt();
// 		Xtring formato = apunte->getRecProyecto()->getValue( "FORMATONUMDOC" ).toString();
// 		apunte->setValue( "NUMDOCUMENTO", apunte->formatNumDocumento( contador, codproyecto, formato ) );
// 	}
// 	apunte->setValue( "FECHADOCUMENTO", fecha );
    apunte->setValue( "DESCRIPCION", descripcion);
    apunte->setValue( "REC_ORIGEN", rec_origen);
    if( old_apunte )
        apunte->rescateValues( old_apunte );
    for( List<ApunteQuickValues>::const_iterator it = apuntevalues.begin();
            it != apuntevalues.end();
            ++it ) {
        contab::RecApunte *apunte = static_cast<contab::RecApunte *>(DBAPP->createRecord( "APUNTE" ));
        apunte->quickValues( Cuenta(it->cuentadebe, getDigitosTrabajo()), it->desc_cuenta_debe,
                             it->debe, it->haber, it->concepto, it->contrapartida );
        apunte->addApunte( apunte );
    }
    FrmEditRec *frmeditapunte = DBAPP->createEditForm(parent,
                                 apunte, 0, DataTable::inserting, dbApplication::simpleEdition, parent );
    if( !supervisar )
        supervisar = !frmeditapunte->showAndSave();
    if( supervisar )
        frmeditapunte->showModalFor( parent, true, true );
    if( frmeditapunte->isSaved() ) {
        DBAPP->getMainWindow()->refreshByName( Xtring::null,Xtring::null );
    }
    delete frmeditapunte;
    return apunte;
}
#endif

/*<<<<<TESORERIAMODULE_FIN*/
} // namespace tesoreria
} // namespace gong

/*>>>>>TESORERIAMODULE_FIN*/