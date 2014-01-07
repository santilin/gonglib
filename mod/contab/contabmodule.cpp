/*<<<<<MODULE_INFO*/
// COPYLEFT Plugin de contabilidad para gestiong
// RECORD Cuenta FrmEditRecMaster Contabilidad
// RECORD Asiento FrmEditRecMaster Contabilidad
// RECORD Apunte FrmEditRecDetail
// NAMESLISTTABLE TipoAsiento
// MODULE_REQUIRED Empresa
// TYPE GongModule contab::ContabModule
/*>>>>>MODULE_INFO*/

/*
	Enlaces al nuevo plan contable 2008

	http://www.boe.es/g/es/bases_datos/doc.php?coleccion=iberlex&id=2007/19884
	http://noticias.juridicas.com/base_datos/Privado/rd1514-2007.html
	Para importar desde la página web, copiar a un fichero de texto y reemplazar:
	s/\s*([0-9]*)\. \s*(.*)$/<REGISTRO-CUENTA id='0'><CUENTA>\1</CUENTA><DESCRIPCION>\2</DESCRIPCION></REGISTRO-CUENTA>/
*/

/*<<<<<CONTABMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include <dbappnameslisttable.h>
#include <dbappfrmeditnameslisttable.h>
#include "contabmodule.h"
#include "contabfrmeditcuenta.h"
#include "contabfrmeditasiento.h"
#include "contabfrmeditapunte.h"
/*>>>>>CONTABMODULE_INCLUDES*/
#include <gongdbrecordlistdatamodel.h>
#include <contactosmodule.h>
#include <empresafldejercicio.h>
#include "contabfrmedittipoivabehavior.h"
#include "contabfrmeditempresa.h"

namespace gong {
namespace contab {

int ContabModule::mLastAsientoID;
int ContabModule::mLastProyectoCodigo; // TODO No se usa
Xtring ContabModule::mLastDocumento;

ContabModule *ModuleInstance;

ContabModule::ContabModule()
    : dbModule( "contab" ), pMenuContabilidad( 0 )
{
    _GONG_DEBUG_TRACE( 1 );
    ModuleInstance = this;
    static dbModuleSetting _settings[] = {
        {
            dbModuleSetting::Bool,
            "SUPERVISAR_ASIENTOS",
            _("Supervisar los asientos que se generan automáticamente"),
            "true"
        },
        // "TIPOASIENTO.VALUES"
        // "CUENTA_CLIENTE"
        // "CUENTA_PROVEEDORA"

        {dbModuleSetting::None}
    };
    pModuleSettings = _settings;
    pEmpresaModule = static_cast<empresa::EmpresaModule *>( DBAPP->findModule( "empresa" ) );
    _GONG_DEBUG_ASSERT( pEmpresaModule );
    mDescription = "Módulo de contabilidad financiera";
    /*<<<<<CONTABMODULE_PUBLIC_INFO*/
	mModuleRequires << "empresa";
	mMasterTables << "CUENTA" << "ASIENTO" << "TIPOASIENTO";
	mDetailTables << "APUNTE";
	pEmpresaModule = static_cast< empresa::EmpresaModule * >(DBAPP->findModule( "Empresa" ));
/*>>>>>CONTABMODULE_PUBLIC_INFO*/
    mImportTables = mMasterTables;
    empresa::ModuleInstance->addContadorTable("ASIENTO");
}


ContabModule::~ContabModule()
{
    _GONG_DEBUG_TRACE( 1 );
}

bool ContabModule::login( FrmLogin *frmlogin, const Xtring &version,
                          Xtring &addtitle, bool startingapp )
{
    return true;
}

void ContabModule::afterLoad()
{
    // (Re)define el estilo CUENTA para que la longitud coincida con el número de dígitos definido en la empresafldejercicio
    dbFieldStyle *cuentastyle = getDatabase()->findFieldStyle( "CUENTA" );
    if( !cuentastyle ) {
        cuentastyle = new dbFieldStyle( "CUENTA" );
        cuentastyle->setHAlignment( dbFieldStyle::AlignLeft );
        pMainDatabase->addFieldStyle( cuentastyle );
    }
    cuentastyle->setWidth( getDigitosTrabajo() );
}

void ContabModule::afterCreateEditForm(FrmEditRec* frm, dbRecord* rec)
{
    if( rec->getTableName() == "TIPOIVA" ) {
        frm->addBehavior( new FrmEditTipoIVABehavior( frm ) );
    }
}

bool ContabModule::isContabActive(empresa::RecEmpresa* recempresa) const
{
    if( recempresa == 0 )
        recempresa = pEmpresaModule->getRecEmpresa();
    return recempresa && recempresa->getValue("CONTABILIDAD").toBool();
}

bool ContabModule::initDatabase( dbDefinition *db )
{
    _GONG_DEBUG_ASSERT( ModuleInstance );   // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

/*<<<<<CONTABMODULE_INIT_DATABASE*/
	pFicTipoAsiento = new NamesListTable( *pMainDatabase, "TIPOASIENTO" );
	pMainDatabase->addTable( pFicTipoAsiento->getTableDefinition() );
/*>>>>>CONTABMODULE_INIT_DATABASE*/

    // Añadir cuentas a las tablas del módulo 'empresa'
    contab::MasterTable *cmt = new contab::MasterTable( db->findTableDefinition( "TIPOIVA" ) );
    cmt->addField<FldCuenta>( "SUBCUENTASOPORTADO" );
    cmt->addField<FldCuenta>( "SUBCUENTAREPERCUTIDO" );
    delete cmt;
    cmt = new contab::MasterTable( db->findTableDefinition( "EMPRESA" ) );
    cmt->addFieldBool( "CONTABILIDAD" );
    cmt->addFieldDate( "INICIOEJERCICIO" );
    cmt->addFieldDate( "FINEJERCICIO" );
    cmt->addFieldDate( "FECHACONTABLE" );
    cmt->addFieldInt( "DIGITOSSUBCUENTAS" );
    cmt->addFieldInt( "MAXNIVELGRUPO" );
    cmt->addFieldString( "CUENTACAJA", 14 );
    delete cmt;

    // Las cuentas son independientes para cada empresa y ejercicio
    pFicCuenta = new contab::MasterTable( *pMainDatabase, "CUENTA" );
    pFicCuenta->addFieldRecordID();
    pFicCuenta->addFieldEmpresaID();
    pFicCuenta->addField<empresa::FldEjercicio>( "EJERCICIO" );
    // La cuenta no es única por los diferentes ejercicios. Ver indice más abajo
    pFicCuenta->addField<FldCuenta>( "CUENTA", dbFieldDefinition::CODE )
    ->setCanBeNull( false )->setUnique( false );
    pFicCuenta->addFieldDesc( "DESCRIPCION", 150 );
    pFicCuenta->addFieldEuro( "SALDO" )->setReadOnly( true );
    pFicCuenta->addFieldEuro( "DEBE" )->setReadOnly( true );
    pFicCuenta->addFieldEuro( "HABER" )->setReadOnly( true );
//     pFicCuenta->addFieldListOfValues( true, getListaTiposCuenta(), getListaTiposCuenta(),
//             "TIPO" )->setCanBeNull( true );
    pFicCuenta->addFieldBool( "MANUAL" );
    pFicCuenta->addFieldNotas();
    pFicCuenta->addFieldOne2OneRelation( "CONTACTO_ID", "CONTACTO.ID", true )
    ->setCaption( DBAPP->getDatabase()->findTableDefinition( "CONTACTO" )->getDescSingular() );
    pFicCuenta->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicCuenta->addMultipleIndex( "cuenta_codigo_unico", "EMPRESA_ID,EJERCICIO,CUENTA", true );
    pMainDatabase->addTable( pFicCuenta->getTableDefinition() );


    pFicApunte = new contab::MasterTable( *pMainDatabase, "APUNTE" );
    pFicApunte->addFieldRecordID();
    pFicApunte->addFieldReferenceID( "ASIENTO_ID", "ASIENTO.ID" );
    pFicApunte->addFieldInt( "NUMAPUNTE" );
    pFicApunte->addFieldOne2OneRelation( "CUENTA_ID", "CUENTA.ID" );
    pFicApunte->addFieldEuro( "DEBE" );
    pFicApunte->addFieldEuro( "HABER" );
    pFicApunte->addFieldString( "CONTRAPARTIDA", 100 );
    pFicApunte->addFieldDesc( "CONCEPTO", 100 )->setCanBeNull( true );
    pFicApunte->addFieldNotas();
    pFicApunte->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicApunte->getTableDefinition() );

    // Los asientos son independientes para cada empresa y ejercicio
    pFicAsiento = new contab::MasterTable( *pMainDatabase, "ASIENTO" );
    pFicAsiento->addFieldRecordID();
    pFicAsiento->addFieldEmpresaID();
    pFicAsiento->addField<empresa::FldEjercicio>( "EJERCICIO" );
    pFicAsiento->addFieldIntCode( "NUMASIENTO" )->setCanBeNull( false )->setUnique( false );
    pFicAsiento->addFieldIntCode( "CONTADOR" )->setUnique(true);
    pFicAsiento->addFieldDate( "FECHA" )->setCanBeNull( false );
    pFicAsiento->addFieldOne2OneRelation( "PROYECTO_ID", "PROYECTO.ID", true );
    pFicAsiento->addFieldDesc( "DESCRIPCION", 100 );
    pFicAsiento->addFieldInt( "CONTADORNUMDOCUMENTO" );
//     pFicAsiento->addFieldString( "NUMDOCUMENTO", 30 );
//     pFicAsiento->addFieldDate( "FECHADOCUMENTO" );
    pFicAsiento->addField<FldNamesListTable>( "TIPOASIENTO" );
    pFicAsiento->addFieldEuro( "SALDO" );
    pFicAsiento->addFieldEuro( "DEBE" );
    pFicAsiento->addFieldEuro( "HABER" );
    pFicAsiento->addFieldBool( "AUTOMATICO" )->setDefaultValue( "0" )->setReadOnly( true );
    pFicAsiento->addFieldNotas();
    pFicAsiento->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pFicAsiento->addRelation( dbRelationDefinition::one2many, "ASIENTO", "ID", "APUNTE", "ASIENTO_ID" );
    pFicAsiento->addMultipleIndex( "numasiento_empresa_ejercicio_unico", "EMPRESA_ID,EJERCICIO,NUMASIENTO", true );
    pMainDatabase->addTable( pFicAsiento->getTableDefinition() );

    return true;
}

/*<<<<<CONTABMODULE_SLOT_CONTABILIDADCUENTA*/
void ContabModule::slotMenuContabilidadCuenta()
{
	pMainWindow->slotMenuEditRecMaestro( "CUENTA" );
}
/*>>>>>CONTABMODULE_SLOT_CONTABILIDADCUENTA*/

/*<<<<<CONTABMODULE_SLOT_CONTABILIDADASIENTO*/
void ContabModule::slotMenuContabilidadAsiento()
{
	pMainWindow->slotMenuEditRecMaestro( "ASIENTO" );
}
/*>>>>>CONTABMODULE_SLOT_CONTABILIDADASIENTO*/

void ContabModule::slotMenuContabilidadDiarioRecalcular()
{
    RecalcularSaldosCuentas( empresa::ModuleInstance->getEjercicio() );
}

void ContabModule::slotMenuContabilidadCuentasManuales()
{
    dbRecord *reccuenta = DBAPP->createRecord( "CUENTA" );
    reccuenta->addStructuralFilter("MANUAL=1");
    pMainWindow->slotMenuEditRecMaestro( "CUENTA", reccuenta, true );
}

dbRecord *ContabModule::createRecord( const Xtring &tablename, dbRecordID recid, dbUser *user )
{
    _GONG_DEBUG_ASSERT( ModuleInstance );   // Assign ModuleInstance to your application
    if ( tablename.upper() == "CONTRAPARTIDA" )
        return new RecCuenta( getConnection(), recid, user );
    /*<<<<<CONTABMODULE_CREATE_RECORD*/
	if( tablename.upper() == "CUENTA" )
		return new RecCuenta(getConnection(), recid, user);
	if( tablename.upper() == "ASIENTO" )
		return new RecAsiento(getConnection(), recid, user);
	if( tablename.upper() == "APUNTE" )
		return new RecApunte(getConnection(), recid, user);
	if( tablename.upper() == "TIPOASIENTO" )
		return new RecNamesListTable("TIPOASIENTO", getConnection(), recid, user);
/*>>>>>CONTABMODULE_CREATE_RECORD*/
    return 0;
}

FrmEditRec *ContabModule::createEditForm( FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        DataTable::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance );   // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    if( tablename.upper() == "EMPRESA" )
        return new contab::FrmEditEmpresa(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
    /*<<<<<CONTABMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "CUENTA" )
		return new FrmEditCuenta(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "ASIENTO" )
		return new FrmEditAsiento(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "TIPOASIENTO" )
		return new FrmEditNamesListTable(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>CONTABMODULE_CREATE_EDITFORM*/
    return 0;
}


FrmEditRecDetail *ContabModule::createEditDetailForm(
    FrmEditRecMaster *frmmaster, int ndetail,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    DataTable::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<CONTABMODULE_CREATE_EDITFORM_DETAIL*/
	if( tablename.upper() == "APUNTE" )
		return new FrmEditApunte(frmmaster, ndetail,
			rec, dettablename, dm, editmode, editflags, parent, name, fl);
/*>>>>>CONTABMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}



bool ContabModule::initMainWindow( MainWindow *mainwin )
{
    _GONG_DEBUG_ASSERT( ModuleInstance );   // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( mainwin );
    pMainWindow = mainwin;

    pMenuContabilidad = new QMenu( pMainWindow );
    pMenuContabilidad->setObjectName( "MenuContabilidad" );
    pMainWindow->menuBar()->insertItem( toGUI( _( "&Contabilidad" ) ), pMenuContabilidad );


#if 0
    text = _( "Extractos de cuentas" );
    pMenuContabilidadExtractos = new QAction(
        text, _( "&Extractos..." ), 0, this );
    pMenuContabilidadExtractos->setStatusTip( text );
    pMenuContabilidadExtractos->setWhatsThis( _( "Muestra extractos de cuentas" ) );
    connect( pMenuContabilidadExtractos, SIGNAL( activated() ), this, SLOT( slotMenuContabilidadExtractos() ) );
    pMenuContabilidadExtractos->addTo( pMenuContabilidad );


    text = _( "Renumeración del diario" );
    pMenuContabilidadDiarioRenumerar = new QAction(
        text, _( "&Renumerar diario..." ), 0, this );
    pMenuContabilidadDiarioRenumerar->setStatusTip( text );
    pMenuContabilidadDiarioRenumerar->setWhatsThis( _( "" ) );
    connect( pMenuContabilidadDiarioRenumerar, SIGNAL( activated() ), this, SLOT( slotMenuContabilidadDiarioRenumerar() ) );
    pMenuContabilidadDiarioRenumerar->addTo( pMenuContabilidadDiario );

    pMenuContabilidadAperturacierre = new Q3PopupMenu();
    pMenuContabilidad->insertItem( _( "Apertura y cierrre" ), pMenuContabilidadAperturacierre );
    text = _( "Crea un nuevo ejercicio" );
    pMenuContabilidadAperturacierreCreaejerc = new QAction(
        text, _( "&Nuevo ejercicio..." ), 0, this );
    pMenuContabilidadAperturacierreCreaejerc->setStatusTip( text );
    pMenuContabilidadAperturacierreCreaejerc->setWhatsThis( _( "" ) );
    connect( pMenuContabilidadAperturacierreCreaejerc, SIGNAL( activated() ), this, SLOT( slotMenuContabilidadAperturacierreCreaejerc() ) );
    pMenuContabilidadAperturacierreCreaejerc->addTo( pMenuContabilidadAperturacierre );

    text = _( "Crea el asiento de apertura" );
    pMenuContabilidadAperturacierreCreaasieaper = new QAction(
        text, _( "&Asiento de apertura..." ), 0, this );
    pMenuContabilidadAperturacierreCreaasieaper->setStatusTip( text );
    pMenuContabilidadAperturacierreCreaasieaper->setWhatsThis( _( "" ) );
    connect( pMenuContabilidadAperturacierreCreaasieaper, SIGNAL( activated() ), this, SLOT( slotMenuContabilidadAperturacierreCreaasieaper() ) );
    pMenuContabilidadAperturacierreCreaasieaper->addTo( pMenuContabilidadAperturacierre );

    pMenuContabilidadAperturacierre->insertSeparator();
    text = _( "Crea el asiento de regularizacion" );
    pMenuContabilidadAperturacierreRegularizar = new QAction(
        text, _( "&Regularizar ejercicio..." ), 0, this );
    pMenuContabilidadAperturacierreRegularizar->setStatusTip( text );
    pMenuContabilidadAperturacierreRegularizar->setWhatsThis( _( "" ) );
    connect( pMenuContabilidadAperturacierreRegularizar, SIGNAL( activated() ), this, SLOT( slotMenuContabilidadAperturacierreRegularizar() ) );
    pMenuContabilidadAperturacierreRegularizar->addTo( pMenuContabilidadAperturacierre );

    text = _( "Crea el asiento de cierre" );
    pMenuContabilidadAperturacierreCerrar = new QAction(
        text, _( "&Cerrar ejercicio..." ), 0, this );
    pMenuContabilidadAperturacierreCerrar->setStatusTip( text );
    pMenuContabilidadAperturacierreCerrar->setWhatsThis( _( "" ) );
    connect( pMenuContabilidadAperturacierreCerrar, SIGNAL( activated() ), this, SLOT( slotMenuContabilidadAperturacierreCerrar() ) );
    pMenuContabilidadAperturacierreCerrar->addTo( pMenuContabilidadAperturacierre );

#endif

    /*<<<<<CONTABMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("CUENTA")->getDescPlural();
		pMenuContabilidadCuenta = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuContabilidadCuenta->setObjectName( "MenuContabilidadCuenta" );
		pMenuContabilidadCuenta->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuContabilidadCuenta->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuContabilidadCuenta, SIGNAL(activated()), this, SLOT(slotMenuContabilidadCuenta()));
		pMenuContabilidadCuenta->addTo(pMenuContabilidad);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("ASIENTO")->getDescPlural();
		pMenuContabilidadAsiento = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuContabilidadAsiento->setObjectName( "MenuContabilidadAsiento" );
		pMenuContabilidadAsiento->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuContabilidadAsiento->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuContabilidadAsiento, SIGNAL(activated()), this, SLOT(slotMenuContabilidadAsiento()));
		pMenuContabilidadAsiento->addTo(pMenuContabilidad);
	}
/*>>>>>CONTABMODULE_INITMAINWINDOW_MENUS*/
    pMenuContabilidadAsiento->setAccel( QKeySequence( "CTRL+SHIFT+D" ) );
    pMenuContabilidadCuenta->setAccel( QKeySequence( "CTRL+SHIFT+T" ) );


    QMenu *pMenuContabilidadDiario = new QMenu();
    pMenuContabilidad->insertItem( toGUI( _( "&Diario" ) ), pMenuContabilidadDiario );

    QString text = _( "Recalcula los saldos de todas las cuentas" );
    pMenuContabilidadDiarioRecalcular = new QAction( _( "Recalcular saldos" ), 0, this, 0 );
    pMenuContabilidadDiarioRecalcular->setStatusTip( text );
    pMenuContabilidadDiarioRecalcular->setWhatsThis( _( "" ) );
    connect( pMenuContabilidadDiarioRecalcular, SIGNAL( activated() ), this, SLOT( slotMenuContabilidadDiarioRecalcular() ) );
    pMenuContabilidadDiarioRecalcular->addTo( pMenuContabilidadDiario );

    text = _( "Cuentas manuales" );
    pMenuContabilidadCuentasManuales = new QAction( _( "Cuentas manuales..." ), 0, this, 0 );
    pMenuContabilidadCuentasManuales->setStatusTip( text );
    pMenuContabilidadCuentasManuales->setWhatsThis( _( "Permite la edición de apuntes en cuentas de forma manual" ) );
    connect( pMenuContabilidadCuentasManuales, SIGNAL( activated() ), this, SLOT( slotMenuContabilidadCuentasManuales() ) );
    pMenuContabilidadCuentasManuales->addTo( pMenuContabilidad );

    return true;
}


int ContabModule::RecalcularSaldosCuentas( int ejercicio )
{
    _GONG_DEBUG_ASSERT( ejercicio );
    DBAPP->waitCursor( true );
#if 0
    int nasientos = getConnection()->selectInt( "SELECT COUNT(*) FROM ASIENTO"
                    " WHERE ejercicio=" + getConnection()->toSQL( ejercicio ) );
    Q3ProgressDialog progress( _( "Recalculando saldos del ejercicio %1..." ).arg( ejercicio ),
                               "Abandonar", nasientos, 0, "progress-recalcular", TRUE );
#endif
    getConnection()->exec( "UPDATE CUENTA SET saldo=0, debe=0, haber=0"
                           " WHERE ejercicio=" + getConnection()->toSQL( ejercicio ) );
    dbResultSet *rsasientos = getConnection()->select( "SELECT id FROM ASIENTO"
                              " WHERE ejercicio=" + getConnection()->toSQL( ejercicio ) );
    RecAsiento asiento( getConnection() );
    while( rsasientos->next() ) {
        asiento.read( rsasientos->toInt( 0 ) );
        for ( unsigned int napunte = 0; napunte < asiento.getApuntes()->size(); napunte++ ) {
            RecApunte *apunte = asiento.getRecApunte( napunte );
            apunte->actSaldosCuenta( true, apunte->getValue( "CUENTA_ID" ).toInt(), ejercicio,
                                     apunte->getValue( "DEBE" ).toMoney(), apunte->getValue( "HABER" ).toMoney() );
        }
#if 0
        progress.setProgress( nasiento++ );
#endif
    }
    DBAPP->getMainWindow()->refreshByName( Xtring::null,Xtring::null );
    DBAPP->resetCursor();
    return 1;
}

#if 0

void GuiMainWindow::hazExtracto( int ejercicio, const QString &cuenta )
{
    if ( cuenta.isEmpty() )
        return ;
    RecAsiento *unasiento = new RecAsiento();
    MainWindow() ->createClient(
        static_cast<FrmBase *>( unasiento->hazExtracto( ejercicio, cuenta ) ), unasiento );
}

void GuiMainWindow::hazExtracto( int ejercicio, RecordID cuentaid )
{
    if ( cuentaid == 0 )
        return ;
    RecAsiento *unasiento = new RecAsiento();
    MainWindow() ->createClient(
        static_cast<FrmBase *>( unasiento->hazExtracto( ejercicio, cuentaid ) ), unasiento );
}

/*GENMENUS_BEG_SLOTMenuContabilidadExtractos*/
void GuiMainWindow::slotMenuContabilidadExtractos()
{
    /*GENMENUS_END_SLOTMenuContabilidadExtractos*/

    hazExtracto( 2004, "5700000000" );
}



#endif

int ContabModule::getDigitosTrabajo() const
{
    return empresa::ModuleInstance->getRecEmpresa()->getValue( "DIGITOSSUBCUENTAS").toInt();
}

uint ContabModule::getMaxNivelGrupo() const
{
    int mng = empresa::ModuleInstance->getRecEmpresa()->getValue( "MAXNIVELGRUPO").toInt();
    if( mng == 0 )
        mng = 4;
    return mng;
}


RecCuenta *ContabModule::creaCuentaTercero( FrmEditRec *parent, dbRecord *tercero,
        const Xtring &prefijo_cuenta, bool supervisar )
{
    Xtring prefijo = prefijo_cuenta;
    if( !prefijo.endsWith(".") )
        prefijo += ".";
    contab::Cuenta cuenta( prefijo + tercero->getValue( "CODIGO" ).toString(),
                           getDigitosTrabajo() );
    cuenta.expandir();
    contab::RecCuenta *reccuenta = static_cast<contab::RecCuenta *>(DBAPP->createRecord( "CUENTA" ));
    reccuenta->clear( true ); // create custom default values
    dbRecordID cuentaid = reccuenta->readWithFilter( "CUENTA=" + reccuenta->getConnection()->toSQL( cuenta ) );
    if( cuentaid == 0 ) {
        if( supervisar )
            parent->msgOk( parent,
                           Xtring::printf( _("La cuenta %s %s %s no existe."), cuenta.c_str(),
                                           DBAPP->getTableDescSingular( tercero->getTableName(), "de la" ).c_str(),
                                           tercero->getValue( "RAZONSOCIAL" ).toString().c_str() ) );
        reccuenta->setValue( "CUENTA", cuenta );
        reccuenta->setValue( "DESCRIPCION", tercero->getValue( "RAZONSOCIAL" ) );
        reccuenta->setValue( "CONTACTO_ID", tercero->getValue( "CONTACTO_ID" ) );
        reccuenta->readRelated( true );
        FrmEditRec *frmeditcuenta = DBAPP->createEditForm(parent,
                                    reccuenta, 0, DataTable::inserting, dbApplication::simpleEdition, parent );
        if( !supervisar )
            supervisar = !frmeditcuenta->showAndSave();
        if( supervisar )
            frmeditcuenta->showModalFor( parent, true, true );
        delete frmeditcuenta;
    }
    return reccuenta;
}

RecAsiento* ContabModule::creaAsientoSimple(FrmEditRec* parent, RecAsiento* old_asiento,
        Date fecha, const Xtring& rec_origen,
        dbRecordID proyecto_id, const Xtring& descripcion,
        const Xtring& concepto, dbRecordID cuentadebe_id, Money debe,
        dbRecordID cuentahaber_id, Money haber, bool supervisar)
{
    RecCuenta *reccuenta = static_cast<RecCuenta *>(DBAPP->createRecord("CUENTA"));
    reccuenta->read( cuentadebe_id );
    Xtring cuentadebe = reccuenta->getValue("CUENTA").toString();
    reccuenta->read( cuentahaber_id );
    Xtring cuentahaber = reccuenta->getValue("CUENTA").toString();
    return creaAsientoSimple( parent, old_asiento, fecha, rec_origen, proyecto_id, descripcion,
                              concepto, cuentadebe, debe, cuentahaber, haber, supervisar );
}


RecAsiento *ContabModule::creaAsientoSimple(FrmEditRec *parent, RecAsiento *old_asiento,
        Date fecha, const Xtring &rec_origen, dbRecordID proyecto_id, const Xtring &descripcion,
        const Xtring &concepto, const Xtring &cuentadebe, Money debe,
        const Xtring &cuentahaber, Money haber,	bool supervisar)
{
    Date fechacontable = empresa::ModuleInstance->getRecEmpresa()->getValue("FECHACONTABLE").toDate();
    if( fechacontable.isValid() && fecha < fechacontable ) {
        DBAPP->showStickyOSD(_("Crear asiento"), _("No se ha creado el asiento porque la fecha es anterior a la fecha contable de esta empresa") );
        return 0;
    }
    DBAPP->waitCursor( true );
    RecAsiento *asiento = static_cast<RecAsiento *>( DBAPP->createRecord("ASIENTO") );
    asiento->clear( true );
    asiento->setValue( "AUTOMATICO", true );
    asiento->setValue( "FECHA", fecha );
    asiento->setValue( "PROYECTO_ID", proyecto_id );
    asiento->setValue( "DESCRIPCION", descripcion );
    asiento->setValue( "REC_ORIGEN", rec_origen );
	asiento->setValue( "TIPOASIENTO", 5 ); // Automático
    if( old_asiento ) {
        asiento->rescateValues( old_asiento );
        asiento->setValue( "ID", old_asiento->getRecordID() );
        old_asiento->remove();
    }
    contab::RecApunte *apuntedebe = static_cast<contab::RecApunte *>(DBAPP->createRecord( "APUNTE" ));
    Cuenta cuentadebe_contraida(cuentadebe, getDigitosTrabajo());
    cuentadebe_contraida.contraer( getMaxNivelGrupo() );
    Cuenta cuentahaber_contraida(cuentahaber, getDigitosTrabajo());
    cuentahaber_contraida.contraer( getMaxNivelGrupo() );
    apuntedebe->quickValues( Cuenta(cuentadebe, getDigitosTrabajo()), Xtring::null, debe, 0,
                             concepto, cuentahaber_contraida );
    asiento->addApunte( apuntedebe );
    contab::RecApunte *apuntehaber = static_cast<contab::RecApunte *>(DBAPP->createRecord( "APUNTE" ));
    apuntehaber->quickValues( Cuenta(cuentahaber, getDigitosTrabajo()), Xtring::null, 0, haber,
                              concepto, cuentadebe_contraida );
    asiento->addApunte( apuntehaber );

    FrmEditRec *frmeditasiento = DBAPP->createEditForm(parent,
                                 asiento, 0, DataTable::inserting, dbApplication::simpleEdition, parent );
    if( !supervisar )
        supervisar = !frmeditasiento->showAndSave();
    if( supervisar )
        frmeditasiento->showModalFor( parent, true, true );
    if( frmeditasiento->isSaved() ) {
        DBAPP->getMainWindow()->refreshByName( Xtring::null,Xtring::null ); // TODO refresh only related
    }
    delete frmeditasiento;
    DBAPP->resetCursor();
    return asiento;
}

RecAsiento *ContabModule::creaAsientoMultiple(FrmEditRec* parent, RecAsiento *old_asiento,
        Date fecha, const Xtring& rec_origen, dbRecordID proyecto_id,
        const Xtring &descripcion,
        List< ApunteQuickValues > apuntevalues, bool supervisar )
{
    Date fechacontable = empresa::ModuleInstance->getRecEmpresa()->getValue("FECHACONTABLE").toDate();
    if( fechacontable.isValid() && fecha < fechacontable ) {
        DBAPP->showStickyOSD(_("Crear asiento"), _("No se ha creado el asiento porque la fecha es anterior a la fecha contable de esta empresa") );
        return 0;
    }
    RecAsiento *asiento = static_cast<RecAsiento *>( DBAPP->createRecord("ASIENTO") );
    asiento->clear( true );
    asiento->setValue( "AUTOMATICO", true );
    asiento->setValue( "FECHA", fecha );
    asiento->setValue( "PROYECTO_ID", proyecto_id );
	asiento->setValue( "TIPOASIENTO", 5 ); // Automático
// 	int contador = asiento->getLastNumDocumento( proyecto_id ) + 1;
// 	asiento->setValue( "CONTADORNUMDOCUMENTO", contador );
// 	if( asiento->getRecProyecto()->read( proyecto_id ) ) {
// 		int codproyecto = asiento->getRecProyecto()->getValue( "CODIGO" ).toInt();
// 		Xtring formato = asiento->getRecProyecto()->getValue( "FORMATONUMDOC" ).toString();
// 		asiento->setValue( "NUMDOCUMENTO", asiento->formatNumDocumento( contador, codproyecto, formato ) );
// 	}
// 	asiento->setValue( "FECHADOCUMENTO", fecha );
    asiento->setValue( "DESCRIPCION", descripcion);
    asiento->setValue( "REC_ORIGEN", rec_origen);
    if( old_asiento )
        asiento->rescateValues( old_asiento );
    for( List<ApunteQuickValues>::const_iterator it = apuntevalues.begin();
            it != apuntevalues.end();
            ++it ) {
        contab::RecApunte *apunte = static_cast<contab::RecApunte *>(DBAPP->createRecord( "APUNTE" ));
        apunte->quickValues( Cuenta(it->cuentadebe, getDigitosTrabajo()), it->desc_cuenta_debe,
                             it->debe, it->haber, it->concepto, it->contrapartida );
        asiento->addApunte( apunte );
    }
    FrmEditRec *frmeditasiento = DBAPP->createEditForm(parent,
                                 asiento, 0, DataTable::inserting, dbApplication::simpleEdition, parent );
    if( !supervisar )
        supervisar = !frmeditasiento->showAndSave();
    if( supervisar )
        frmeditasiento->showModalFor( parent, true, true );
    if( frmeditasiento->isSaved() ) {
        DBAPP->getMainWindow()->refreshByName( Xtring::null,Xtring::null );
    }
    delete frmeditasiento;
    return asiento;
}


/*<<<<<CONTABMODULE_FIN*/
} // namespace contab
} // namespace gong

/*>>>>>CONTABMODULE_FIN*/

