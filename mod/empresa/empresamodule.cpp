/*<<<<<MODULE_INFO*/
// COPYLEFT Módulo de empresas comerciales de GongLib
// RECORD Empresa FrmEditRecMaster Empresa
// RECORD Departamento FrmEditRecMaster Empresa
// RECORD Moneda FrmEditRecMaster Empresa
// RECORD TipoIVA FrmEditRecMaster Empresa
// RECORD Proyecto FrmEditRecMaster Empresa
// MODULE_REQUIRED Contactos
// TYPE GongModule empresa::EmpresaModule
/*>>>>>MODULE_INFO*/

/*
 * Los formularios para facturas, albaranes y pedidos podrían tener un campo para elegir la empresa
 * de modo que si se cambia de empresa uno que ya está creado, se iría a la otra empresa.
 *
 * Lo he intentado y casi conseguido, pero he llegado a un problema difícil de resolver. Al cambiar una
 * factura de empresa, el filtro del registro que se está editando se creó con la empresa anterior, por
 * lo que las búsquedas de máximos y comprobaciones sobre ese registro toman la empresa anterior.
 * Esto hace difícil comprobar que la factura no esté duplicada en otra empresa.
 * También añade una relación más a estos ficheros, que no es necesario si no se puede modificar
 * la empresa
 *
 * Y creo que saldrán muchos problemas más, así que he decidido deshacer lo que había hecho en esos
 * formularios y crearé después opciones para copiar facturas, albaranes, pedidos y asientos entre
 * empresas.
 *
 */

/*<<<<<EMPRESAMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "empresamodule.h"
#include "empresafrmeditempresa.h"
#include "empresafrmeditdepartamento.h"
#include "empresafrmeditmoneda.h"
#include "empresafrmedittipoiva.h"
#include "empresafrmeditproyecto.h"
/*>>>>>EMPRESAMODULE_INCLUDES*/
#include <dbappfrmcustom.h>
#include <contactosmodule.h>
#include <gongdbfieldimage.h>
#include "empresamastertable.h"
#ifdef HAVE_RTKMODULE
# include <dbappreport.h>
#endif
namespace gong {
namespace empresa {

EmpresaModule *ModuleInstance = 0;

static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::Bool,
        "USAR_MAXCONTADOR",
        _("Usar contador único para las compras y gastos"),
        "false",
        dbModuleSetting::Global
    },
    { dbModuleSetting::None }
};

EmpresaModule::EmpresaModule()
    : dbModule( "empresa" ), pRecEmpresa( 0 ), mCodEmpresa( 0 ), mEjercicio( 0 )
{
    ModuleInstance = this;
    pModuleSettings = _settings;
    _GONG_DEBUG_TRACE( 1 );
    mDescription = "Módulo de definición de la organización";
    /*<<<<<EMPRESAMODULE_PUBLIC_INFO*/
	mModuleRequires << "contactos";
	mMasterTables << "EMPRESA" << "DEPARTAMENTO" << "MONEDA" << "TIPOIVA" << "PROYECTO";
//	mDetailTables
	pContactosModule = static_cast< contactos::ContactosModule * >(DBAPP->findModule( "Contactos" ));
/*>>>>>EMPRESAMODULE_PUBLIC_INFO*/
}


EmpresaModule::~EmpresaModule()
{
    _GONG_DEBUG_TRACE( 1 );
}

void EmpresaModule::setCodEmpresa( int cod )
{
    mCodEmpresa = cod;
}

int EmpresaModule::getCodEmpresa() const
{
    return mCodEmpresa;
}

bool EmpresaModule::isReadOnly() const
{
    _GONG_DEBUG_ASSERT( pRecEmpresa );
    return pRecEmpresa->getValue( "SOLOLECTURA" ).toBool();
}

Xtring EmpresaModule::getNombreEmpresa() const
{
    _GONG_DEBUG_ASSERT( pRecEmpresa );
    return pRecEmpresa->getValue( "NOMBRE" ).toString();
}

RecEmpresa *EmpresaModule::getRecEmpresa() const
{
    _GONG_DEBUG_ASSERT( pRecEmpresa );
    return pRecEmpresa;
}

uint EmpresaModule::getDecimalesMoneda() const
{
    if( getRecEmpresa() && getRecEmpresa()->getRecMoneda() ) {
        return getRecEmpresa()->getRecMoneda()->getValue( "DECIMALES" ).toInt();
    } else {
        return 2;
    }
}

bool EmpresaModule::usaProyectos() const
{
    return getRecEmpresa()->getValue( "USARPROYECTOS" ).toBool();
}


bool EmpresaModule::login( FrmLogin *frmlogin, const Xtring &version,
                           Xtring &addtitle, bool startingapp )
{
    setCodEmpresa( DBAPP->getAppSetting( "CodEmpresa", 1 ).toInt() );
    if( !startingapp )
        setEjercicio( DBAPP->getAppSetting( "Ejercicio", Date::currentDate().getYear() ).toInt() );
    else
        setEjercicio( Date::currentDate().getYear() );
    pConnection = DBAPP->getConnection();
    Variant nombre(Variant::tString), sololectura(Variant::tBool);
    getConnection()->selectValues( "SELECT NOMBRE, SOLOLECTURA FROM EMPRESA WHERE CODIGO="
                                   + getConnection()->toSQL( getCodEmpresa() ), &nombre, &sololectura);
    if( nombre.toString().isEmpty() ) {
        nombre = Xtring::printf( _("%s %d - %s"),
                                 DBAPP->getTableDescSingular("EMPRESA", "").proper().c_str(),
                                 getCodEmpresa(), DBAPP->getPackageString().c_str() );
        if( getConnection()->exec( "INSERT INTO EMPRESA(CODIGO,NOMBRE)VALUES("
                                   + getConnection()->toSQL( getCodEmpresa() ) + ","
                                   + getConnection()->toSQL( nombre.toString() ) + ")" ) == 0 ) {
            FrmBase::msgError( DBAPP->getPackageString(),
                               Xtring::printf(_("No se ha podido crear %s de código %d.\n%s terminará ahora"),
                                              DBAPP->getTableDescSingular("EMPRESA", "la").c_str(),
                                              getCodEmpresa(), DBAPP->getPackageString().c_str()) );
            exit( 1 );
        } else {
            DBAPP->showOSD( DBAPP->getPackageString(),
                            Xtring::printf( _("Se ha creado %s '%s'."),
                                            DBAPP->getTableDescSingular("EMPRESA", "la").c_str(),
                                            nombre.toString().c_str() ) );
        }
    } else {
        DBAPP->showStickyOSD( DBAPP->getPackageString(),
                        Xtring::printf("Trabajando con %s %d, '%s', ejercicio %d",
                                       DBAPP->getTableDescSingular("EMPRESA", "la").c_str(),
                                       getCodEmpresa(),
                                       nombre.toString().c_str(),
                                       getEjercicio() ) );
    }
    DBAPP->setReadOnly( sololectura.toBool() || getCodEmpresa() == 0 );
    addtitle = "[" + nombre.toString() + ", " + Xtring::number(getEjercicio()) + "]";
    return true;
}

void EmpresaModule::slotMenuEmpresaCambiarEjercicio()
{
    Xtring caption = Xtring::printf( _("Cambiar de %s y/o ejercicio"),
                                     DBAPP->getTableDescSingular("EMPRESA", "").c_str() );
    class FrmCambiarEjercicio: public FrmCustom
    {
    public:
        FrmCambiarEjercicio(const Xtring &caption, int codempresa, int ejercicio )
            : FrmCustom( 0, "FrmCambiarEjercicio" )
        {
            setTitle( caption );
            pSearchEjercicios = addButton(0, _("¿Ejercicios?") );
            pSearchEmpresa = addSearchField( 0, "Empresa", "CODIGO", "NOMBRE" );
            pSearchEmpresa->getEditCode()->setWidthInChars(4);
            pSearchEmpresa->setValue( codempresa );
            pSearchEmpresa->getEditCode()->setSelectedOnEntry( true );
            pEditEjercicio = addInput( 0, _("Ejercicio"), Variant( ejercicio ), "INTEGER" );
            pEditEjercicio->setSelectedOnEntry( true );
            pFocusWidget = pSearchEmpresa->getEditCode();
        }
        void validate_input( QWidget *sender, bool *isvalid ) { // from FrmCustom
            if( sender == pSearchEjercicios ) {
                IntList ejercicios;
                ModuleInstance->getEjercicios( ejercicios );
                XtringList sejercicios;
                for( IntList::const_iterator it = ejercicios.begin(); it!=ejercicios.end(); ++it)
                    sejercicios << Xtring::number( *it );
                int elegido = msgXtringList( this, _("Elige uno de los ejercicios para los que hay datos"), sejercicios );
                if( elegido != -1 ) {
                    elegido = 2010 + elegido;
                    pEditEjercicio->setText( elegido );
                }
            }
        }
        int getCodEmpresa() const {
            return pSearchEmpresa->getEditCode()->toInt();
        }
        int getEjercicio() const {
            return pEditEjercicio->toInt();
        }
    private:
        PushButton *pSearchEjercicios;
        LineEdit *pEditEjercicio;
        SearchBox *pSearchEmpresa;
    };
    if ( DBAPP->getMainWindow()->getNumClients() == 0 ) {
        FrmCambiarEjercicio *frmcambiar = new FrmCambiarEjercicio(
            caption, getCodEmpresa(), getEjercicio() );
        frmcambiar->showModalFor( 0, true, true );
        if( !frmcambiar->wasCancelled() ) {
            int emp = frmcambiar->getCodEmpresa(), eje = frmcambiar->getEjercicio();
            if ( emp < 1 || emp > 99999 )
            {
                FrmBase::msgOk( frmcambiar,
                                Xtring::printf( _("Introduce un código de %s entre 1 y 99999" ),
                                                DBAPP->getTableDescSingular("EMPRESA", "la").c_str() ) );
                return;
            }
            if( eje == 0 )  {
                FrmBase::msgOk( frmcambiar, _("Has seleccionado el ejercicio 0. Esto significa que tendrás acceso a todos los ejercicios a la vez.") );
            } else if( eje < 1900 || frmcambiar->getEjercicio() > 2200 ) {
                FrmBase::msgError(frmcambiar, _("Introduce un número de ejercicio razonable" ) );
                return ;
            } else if ( eje < 100 && eje > 0 )
            {
                setEjercicio ( eje + 2000 );
            } else {
                setEjercicio( eje );
            }
            DBAPP->waitCursor( true );
            setCodEmpresa( emp );
            DBAPP->setUserLocalSetting( "CodEmpresa", getCodEmpresa() );
            DBAPP->setUserLocalSetting( "Ejercicio", getEjercicio() );
            DBAPP->login( PACKAGE_VERSION, false, true );
            rereadEmpresa();
            DBAPP->setTitle();
            DBAPP->resetCursor();
        } else {
            delete frmcambiar;
        }
    } else {
        FrmBase::msgOk( caption,
                        Xtring::printf( _( "Debes cerrar todas las ventanas antes de cambiar de %s" ),
                                        DBAPP->getTableDescSingular("EMPRESA", "").c_str() ), FrmBase::information );
    }
}


void EmpresaModule::slotMenuEmpresaEjercicioAnterior()
{
	DBAPP->hideOSD();
    DBAPP->waitCursor( true );
    DBAPP->setUserLocalSetting( "Ejercicio", getEjercicio() - 1);
    DBAPP->login( PACKAGE_VERSION, false, true );
    rereadEmpresa();
    DBAPP->setTitle();
    DBAPP->resetCursor();
}

void EmpresaModule::slotMenuEmpresaEjercicioSiguiente()
{
	DBAPP->hideOSD();
    DBAPP->waitCursor( true );
    DBAPP->setUserLocalSetting( "Ejercicio", getEjercicio() + 1);
    DBAPP->login( PACKAGE_VERSION, false, true );
    rereadEmpresa();
    DBAPP->setTitle();
    DBAPP->resetCursor();
}

bool EmpresaModule::initDatabase( dbDefinition *db )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

    // La tabla MONEDA es común a todas las empresas y ejercicios
    pFicMoneda = new empresa::MasterTable( *pMainDatabase, "MONEDA" );
    pFicMoneda->addFieldRecordID();
    pFicMoneda->addFieldCodigo();
    pFicMoneda->addFieldDesc( "NOMBRE" )->setUnique( true );
    pFicMoneda->addFieldString( "SIMBOLO", 10 );
    pFicMoneda->addFieldDate( "ACTUALIZACION" );
    pFicMoneda->addFieldFloat( "TASACAMBIO" );
    pFicMoneda->addFieldInt( "DECIMALES" );
    pFicMoneda->addFieldChar( "PUNTODECIMAL" );
    pFicMoneda->addFieldChar( "SEPARADORMILES" );
    pFicMoneda->addFieldString( "FORMATOMONEDA", 10 );
    pFicMoneda->addFieldNotas();
    pFicMoneda->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicMoneda->getTableDefinition() );

    // La tabla DEPARTAMENTO es común a todas las empresas y ejercicios
    pFicDepartamento = new empresa::MasterTable( *pMainDatabase, "DEPARTAMENTO" );
    pFicDepartamento->addFieldRecordID();
    pFicDepartamento->addFieldCodigo()->setUnique( false );
    pFicDepartamento->addFieldDesc("NOMBRE")->setUnique( true );
    pFicDepartamento->addFieldNotas();
    pFicDepartamento->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicDepartamento->getTableDefinition() );

    // La tabla TIPOIVA es común a todas las empresas y ejercicios
    pFicTipoIVA = new empresa::MasterTable( *pMainDatabase, "TIPOIVA" );
    pFicTipoIVA->addFieldRecordID();
    pFicTipoIVA->addFieldCodigo();
    pFicTipoIVA->addFieldStringCode( "NOMBRE" );
    pFicTipoIVA->addFieldPercent( "IVA" );
    pFicTipoIVA->addFieldPercent( "RECARGO" );
    pFicTipoIVA->addFieldNotas();
    pFicTipoIVA->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicTipoIVA->getTableDefinition() );


    pFicProyecto = new empresa::MasterTable( *pMainDatabase, "PROYECTO" );
    pFicProyecto->addFieldRecordID();
    pFicProyecto->addFieldCodigo();
    pFicProyecto->addFieldDesc( "NOMBRE" )->setUnique( true );
    pFicProyecto->addFieldNotas();
    pMainDatabase->addTable( pFicProyecto->getTableDefinition() );

    // La tabla EMPRESA es común a todos los ejercicios
    pFicEmpresa = new empresa::MasterTable( *pMainDatabase, "EMPRESA" );
    pFicEmpresa->addFieldRecordID();
    // Contaplus: Clave: para evitar que otros usuarios accedan a esta empresa
    //            Nivel de seguridad para restringir a los usuarios el acceso a esta empresa
    //            Datos del registro mercantil
    //            Nombre del administrador
    //			  Representante legal para menores de 14 años
    // 			  Fecha última liquidación del IVA (CP86)
    //			  Número de asientos de apertura, regularización y cierre (CP86)
    pFicEmpresa->addFieldCodigo();
    pFicEmpresa->addFieldDesc( "NOMBRE", 120 )->setUnique( true );
    pFicEmpresa->addFieldBool( "SOLOLECTURA" );
    // Hasta aquí, estos campos son obligatorios para poder hacer login de la empresa
    pFicEmpresa->addFieldBool( "USARPROYECTOS" );
    pFicEmpresa->addFieldAggregateRelation( "CONTACTO_ID", "CONTACTO.ID", true );
    pFicEmpresa->addFieldOne2OneRelation( "MONEDA_ID", "MONEDA.ID", true );
    pFicEmpresa->addFieldString("ENTIDADBANCO", 80);
    pFicEmpresa->addField<FldCuentaBanco>( "CUENTABANCO" );
    pFicEmpresa->addFieldNotas();
    pFicEmpresa->addField<dbFieldImage>( "LOGO" );
    pFicEmpresa->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicEmpresa->getTableDefinition() );

    return true;
}

dbRecord *EmpresaModule::createRecord( const Xtring &tablename, dbRecordID recid, dbUser *user )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    /*<<<<<EMPRESAMODULE_CREATE_RECORD*/
	if( tablename.upper() == "EMPRESA" )
		return new RecEmpresa(getConnection(), recid, user);
	if( tablename.upper() == "DEPARTAMENTO" )
		return new RecDepartamento(getConnection(), recid, user);
	if( tablename.upper() == "MONEDA" )
		return new RecMoneda(getConnection(), recid, user);
	if( tablename.upper() == "TIPOIVA" )
		return new RecTipoIVA(getConnection(), recid, user);
	if( tablename.upper() == "PROYECTO" )
		return new RecProyecto(getConnection(), recid, user);
/*>>>>>EMPRESAMODULE_CREATE_RECORD*/
    return 0;
}


FrmEditRec *EmpresaModule::createEditForm( FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        DataTable::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<EMPRESAMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "EMPRESA" )
		return new FrmEditEmpresa(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "DEPARTAMENTO" )
		return new FrmEditDepartamento(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "MONEDA" )
		return new FrmEditMoneda(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "TIPOIVA" )
		return new FrmEditTipoIVA(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "PROYECTO" )
		return new FrmEditProyecto(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>EMPRESAMODULE_CREATE_EDITFORM*/
    return 0;
}

FrmEditRecDetail *EmpresaModule::createEditDetailForm(
    FrmEditRecMaster *frmmaestro, int ndetalle,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    DataTable::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<EMPRESAMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>EMPRESAMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}

/*<<<<<EMPRESAMODULE_SLOT_EMPRESATIPOIVA*/
void EmpresaModule::slotMenuEmpresaTipoIVA()
{
	pMainWindow->slotMenuEditRecMaestro( "TIPOIVA" );
}
/*>>>>>EMPRESAMODULE_SLOT_EMPRESATIPOIVA*/
/*<<<<<EMPRESAMODULE_SLOT_EMPRESAEMPRESA*/
void EmpresaModule::slotMenuEmpresaEmpresa()
{
	pMainWindow->slotMenuEditRecMaestro( "EMPRESA" );
}
/*>>>>>EMPRESAMODULE_SLOT_EMPRESAEMPRESA*/
/*<<<<<EMPRESAMODULE_SLOT_EMPRESADEPARTAMENTO*/
void EmpresaModule::slotMenuEmpresaDepartamento()
{
	pMainWindow->slotMenuEditRecMaestro( "DEPARTAMENTO" );
}
/*>>>>>EMPRESAMODULE_SLOT_EMPRESADEPARTAMENTO*/
/*<<<<<EMPRESAMODULE_SLOT_EMPRESAMONEDA*/
void EmpresaModule::slotMenuEmpresaMoneda()
{
	pMainWindow->slotMenuEditRecMaestro( "MONEDA" );
}
/*>>>>>EMPRESAMODULE_SLOT_EMPRESAMONEDA*/
/*<<<<<EMPRESAMODULE_SLOT_EMPRESAPROYECTO*/
void EmpresaModule::slotMenuEmpresaProyecto()
{
	pMainWindow->slotMenuEditRecMaestro( "PROYECTO" );
}
/*>>>>>EMPRESAMODULE_SLOT_EMPRESAPROYECTO*/


void EmpresaModule::afterLoad()
{
    if ( !pRecEmpresa )
        pRecEmpresa = static_cast<RecEmpresa *>( DBAPP->createRecord( "EMPRESA" ) );
	rereadEmpresa();
}


bool EmpresaModule::initMainWindow( MainWindow *mainwin )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( mainwin );
    pMainWindow = mainwin;
    pMenuEmpresa = new QMenu( pMainWindow );
    pMenuEmpresa->setObjectName( "MenuEmpresa" );
    pMainWindow->menuBar()->insertItem( toGUI( "&" + DBAPP->getTableDescSingular("EMPRESA", "") ),
                                        pMenuEmpresa );
    {
        QString text = toGUI( Xtring::printf( _("Cambiar de %s y/o ejercicio"),
                                              DBAPP->getTableDescSingular("EMPRESA", "").c_str() ).c_str() );
        pMenuEmpresaCambiarEjercicio = new QAction(text + "...", 0, pMainWindow,0);
        pMenuEmpresaCambiarEjercicio->setStatusTip(text);
        pMainWindow->connect(pMenuEmpresaCambiarEjercicio, SIGNAL(activated()), this, SLOT(slotMenuEmpresaCambiarEjercicio()));
        pMenuEmpresaCambiarEjercicio->addTo(pMenuEmpresa);
    }
    {
        QString text = toGUI( _("Ejercicio anterior") );
        pMenuEmpresaEjercicioAnterior = new QAction(text + "...", 0, pMainWindow,0);
        pMenuEmpresaEjercicioAnterior->setStatusTip(text);
        pMainWindow->connect(pMenuEmpresaEjercicioAnterior, SIGNAL(activated()), this, SLOT(slotMenuEmpresaEjercicioAnterior()));
        pMenuEmpresaEjercicioAnterior->addTo(pMenuEmpresa);
    }
    {
        QString text = toGUI( _("Ejercicio siguiente") );
        pMenuEmpresaEjercicioSiguiente = new QAction(text + "...", 0, pMainWindow,0);
        pMenuEmpresaEjercicioSiguiente->setStatusTip(text);
        pMainWindow->connect(pMenuEmpresaEjercicioSiguiente, SIGNAL(activated()), this, SLOT(slotMenuEmpresaEjercicioSiguiente()));
        pMenuEmpresaEjercicioSiguiente->addTo(pMenuEmpresa);
    }
/*<<<<<EMPRESAMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = "Mi " + DBAPP->getDatabase()->findTableDefinition("EMPRESA")->getDescSingular();
		pMenuEmpresaEmpresa = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuEmpresaEmpresa->setObjectName( "MenuEmpresaEmpresa" );
		pMenuEmpresaEmpresa->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuEmpresaEmpresa->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuEmpresaEmpresa, SIGNAL(activated()), this, SLOT(slotMenuEmpresaEmpresa()));
		pMenuEmpresaEmpresa->addTo(pMenuEmpresa);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("DEPARTAMENTO")->getDescPlural();
		pMenuEmpresaDepartamento = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuEmpresaDepartamento->setObjectName( "MenuEmpresaDepartamento" );
		pMenuEmpresaDepartamento->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuEmpresaDepartamento->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuEmpresaDepartamento, SIGNAL(activated()), this, SLOT(slotMenuEmpresaDepartamento()));
		pMenuEmpresaDepartamento->addTo(pMenuEmpresa);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("MONEDA")->getDescPlural();
		pMenuEmpresaMoneda = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuEmpresaMoneda->setObjectName( "MenuEmpresaMoneda" );
		pMenuEmpresaMoneda->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuEmpresaMoneda->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuEmpresaMoneda, SIGNAL(activated()), this, SLOT(slotMenuEmpresaMoneda()));
		pMenuEmpresaMoneda->addTo(pMenuEmpresa);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("TIPOIVA")->getDescPlural();
		pMenuEmpresaTipoIVA = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuEmpresaTipoIVA->setObjectName( "MenuEmpresaTipoIVA" );
		pMenuEmpresaTipoIVA->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuEmpresaTipoIVA->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuEmpresaTipoIVA, SIGNAL(activated()), this, SLOT(slotMenuEmpresaTipoIVA()));
		pMenuEmpresaTipoIVA->addTo(pMenuEmpresa);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("PROYECTO")->getDescPlural();
		pMenuEmpresaProyecto = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuEmpresaProyecto->setObjectName( "MenuEmpresaProyecto" );
		pMenuEmpresaProyecto->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuEmpresaProyecto->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuEmpresaProyecto, SIGNAL(activated()), this, SLOT(slotMenuEmpresaProyecto()));
		pMenuEmpresaProyecto->addTo(pMenuEmpresa);
	}
/*>>>>>EMPRESAMODULE_INITMAINWINDOW_MENUS*/
/* {capel}
		Xtring caption = "Mi " + DBAPP->getDatabase()->findTableDefinition("EMPRESA")->getDescSingular();
*/
    pContactosModule->getMenuContactos()->addTo( pMenuEmpresa );
    QAction *menucontactosmailing = DBAPP->getMainWindow()->findAction( "MenuContactosMailing" );
    if( menucontactosmailing )
        menucontactosmailing->addTo( pMenuEmpresa );
    rereadEmpresa();
    return true;
}

void EmpresaModule::rereadEmpresa()
{
    // Esto se hace aquí y no en el afterLoad porque necesitamos pMainWindow
    // Esto va aquí porque en el login podría ocurrir que haya campos que no existan aún
    if( !pRecEmpresa->read( "CODIGO=" + getConnection()->toSQL( getCodEmpresa() ) ) ) {
        FrmBase::msgOk( DBAPP->getPackageString(),
                        Xtring::printf(_( "%s de código %d no existe y no ha sido creada.\n%s terminará ahora"),
                                       DBAPP->getTableDescSingular("EMPRESA", "la").c_str(),
                                       getCodEmpresa(), DBAPP->getPackageString().c_str() ), FrmBase::information );
        exit( 1 );
    }
#ifdef HAVE_RTKMODULE
    AppReport::setReportAuthor( DBAPP->getPackageString() );
    AppReport::setReportCompany( getNombreEmpresa() );
    AppReport::setReportSubTitle( getNombreEmpresa() );
#endif
    if ( !pRecEmpresa->getRecMoneda()->getRecordID() || !pRecEmpresa->getRecContacto()->getRecordID() ) {
        theGuiApp->waitCursor( true );
        if ( !pRecEmpresa->getRecMoneda()->getRecordID() ) {
            dbRecordID monedaid = getConnection()->selectInt( "SELECT MIN(ID) FROM MONEDA" );
            if( monedaid != 0 ) {
                pRecEmpresa->setValue( "MONEDA_ID", monedaid );
                pRecEmpresa->readRelated( true );
                pRecEmpresa->getRecMoneda()->setRegConfigFromValues( *DBAPP->getRegConfig() );
            } else {
                pRecEmpresa->getRecMoneda()->clear( true );
                pRecEmpresa->getRecMoneda()->setValue( "NOMBRE", "Moneda" );
                pRecEmpresa->getRecMoneda()->setValue( "CODIGO", 1 );
                pRecEmpresa->getRecMoneda()->setValuesFromRegConfig( *DBAPP->getRegConfig() );
                pRecEmpresa->getRecMoneda()->save(false);
                pRecEmpresa->setValue( "MONEDA_ID", pRecEmpresa->getRecMoneda()->getRecordID() );
                pRecEmpresa->save(false);
                DBAPP->showStickyOSD( DBAPP->getPackageString(),
                                      _("Se ha definido una moneda por defecto, revísala para confirmar que es la moneda que quieres utilizar.") );
            }
        }
        if( !pRecEmpresa->getRecContacto()->getRecordID() ) {
            DBAPP->showStickyOSD( DBAPP->getPackageString(),
                                  Xtring::printf(_("%s no tiene definidos sus datos de contacto. El programa no funcionará correctamente hasta que los definas."),
                                                 DBAPP->getTableDescSingular("EMPRESA", "la").c_str() ) );
        }
        DBAPP->resetCursor();
    }
}

Variant EmpresaModule::getGlobalProperty(const Xtring& propertyname) const
{
    Xtring tblname, fldname;
    propertyname.upper().splitIn2(tblname, fldname, ".");
    if( tblname == "EMPRESA" ) {
        if( pRecEmpresa ) {
            return pRecEmpresa->getValue( fldname );
        } else {
            _GONG_DEBUG_WARNING( "pRecEmpresa has not been read" );
        }
    }
    return Variant();
}

void EmpresaModule::addContadorTable(const Xtring& tablename)
{
    if( !mContadorTables.contains( tablename ) )
        mContadorTables << tablename;
}

int EmpresaModule::getMaxContador() const
{
    int max = 0;
    if( getModuleSetting( "USAR_MAXCONTADOR" ).toBool() == false )
        return max;
    if( mContadorTables.size() ) {
        for( XtringList::const_iterator it = mContadorTables.begin();
                it != mContadorTables.end(); ++it ) {
            try {
                dbRecord *rec = DBAPP->createRecord( *it );
                int tblmax = rec->selectNextInt("CONTADOR", Xtring::null, false); // don't use gaps
                delete rec;
                if( tblmax > max )
                    max = tblmax;
                _GONG_DEBUG_PRINT(4, "Max contador: " + Xtring::number( tblmax ) );
            } catch( dbError &e ) {
                _GONG_DEBUG_WARNING( e.what() );
            }
        }
    }
    return max;
}

int EmpresaModule::getEjercicios(IntList& ejercicios) const
{
    // Buscar todas las tablas que tienen ejercicio
    DBAPP->waitCursor( true );
    uint eje_min = Date::currentDate().getYear(), eje_max = eje_min;
    dbTableDefinitionDict tables = DBAPP->getDatabase()->getTables();
    for( dbTableDefinitionDict::const_iterator it = tables.begin(); it != tables.end(); ++it ) {
        dbTableDefinition *tbldef = it->second;
        FldEjercicio *fldeje = dynamic_cast<FldEjercicio *>(tbldef->findFieldDefinition( "EJERCICIO" ));
        if( fldeje ) {
            Variant min = 0, max = 0;
            if( DBAPP->getConnection()->selectValues(
                        "SELECT MIN(" + DBAPP->getConnection()->nameToSQL("EJERCICIO") + "),"
                        " MAX(" + DBAPP->getConnection()->nameToSQL("EJERCICIO") + ")"
                        " FROM " + DBAPP->getConnection()->nameToSQL( tbldef->getName() ),
                        &min, &max ) ) {
                if( min.toUInt() != 0 && min.toUInt() < eje_min )
                    eje_min = min.toUInt();
                if( max.toUInt() != 0 && max.toUInt() < eje_max )
                    eje_max = max.toUInt();
            }
        }
    }
    ejercicios.clear();
    for( uint eje = eje_min; eje <= eje_max; ++eje )
        ejercicios << eje;
    DBAPP->resetCursor();
    return ejercicios.size();
}


/*<<<<<EMPRESAMODULE_FIN*/
} // namespace empresa
} // namespace gong

/*>>>>>EMPRESAMODULE_FIN*/

/*<<<<<EMPRESAMODULE_INIT_DATABASE*/

/*>>>>>EMPRESAMODULE_INIT_DATABASE*/
