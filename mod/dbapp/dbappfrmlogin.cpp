/**
	\todo {0.3.4} Poner en rojo los mensajes de error
	\todo {0.3.4} No activar el botón crear base de datos al principio
	\todo {0.3.4} Encriptar la contraseña en settings.rc ?
*/


#include <unistd.h>
#include <cstdlib>              // getenv
#include <cstdio>

#include <qcombobox.h>

#include <gongguicontrols.h>
#include <gonggettext.h>
#include <gongfrmbase.h>
#include "dbappdbapplication.h"
#include "dbappfrmlogin.h"

namespace gong {

inline Xtring getRealUser(const Xtring &user)
{
// 	if( user != "root" )
// 		return "gong" + Xtring::number ( codasoc ).padL ( 4, '0' ) + "_" + user;
// 	else
    return user;
}

const char *loginInstructions = gettext_noop(
                                    "Introduce los datos para conectarte a la base de datos de %1s.\n\n"
                                    "   Si la base de datos ya existe, introduce los datos necesarios para encontrarla, es decir,"
                                    " el USUARIO/A, la CONTRASEÑA y en qué ordenador (SERVIDOR) se encuentra:\n"
                                    "   * Si la base de datos está en este ordenador, pulsa el botón 'SIGUIENTE'.\n"
                                    "   * Si está en otro ordenador, escribe el nombre o dirección IP del otro ordenador en el campo SERVIDOR y opcionalmente el PUERTO y pulsa el botón SIGUIENTE.\n"
                                    "   * Si la base de datos no existe, pulsa el botón CREAR BASE DE DATOS.\n\n");

const char *loginWelcome = gettext_noop("Bienvenida a %s");

/*
  En realidad este diálogo es como una splash screen, siempre se muestra
  aunque estemos autoconectando.
*/
FrmLogin::FrmLogin ( bool startingapp, QWidget* parent, const char* name, WidgetFlags fl )
    : GuiFrmLogin ( parent, name, fl ),
      mStartingApp( startingapp )
{
    XtringList::iterator it = dbConnection::getDriversList().begin();
    while ( it != dbConnection::getDriversList().end() )
    {
        comboDriver->insertItem ( toGUI(*it) );
        comboCreateDBDriver->insertItem ( toGUI(*it) );
        ++it;
    }
    if ( comboDriver->count() == 0 )
    {
        FrmBase::msgError( this,
                           _( "No se han encontrado drivers en este ordenador para la conexión a la base de datos SQL.\n"
                              "El programa finalizará ahora.") );
        _GONG_DEBUG_WARNING( "No se encontraron drivers SQL" );
        exit(0);
    }
    connect ( pushCreateDatabase, SIGNAL ( clicked() ), SLOT ( slotCreateDatabase() ) );
    setCaption( toGUI("Asistente de acceso a " + DBAPP->getPackageString() ) );
}

bool FrmLogin::init()
{
    show();
    setDriver ( DBAPP->getAppSetting ( "DBDRIVER", "MYSQL" ).toString() );
    setHost ( DBAPP->getAppSetting ( "DBHOST" ).toString() );
    setDBName ( DBAPP->getAppSetting ( "DBNAME" ).toString() );
    setPort ( DBAPP->getAppSetting ( "DBPORT" ).toInt() );
    setUser ( DBAPP->getAppSetting ( "DBUSER" ).toString() );
    bool savepass = DBAPP->getAppSetting ( "DBSAVEPASSWORD", false ).toBool();
    setSavePassword ( savepass );
    if ( savepass )
        setPassword ( DBAPP->getAppSetting ( "DBPASSWORD" ).toString() );
    pushCreateDatabase->setEnabled ( true );
    mStatus = init_login;
    // Seleccionar el driver en el combo
    int i;
    for ( i = 0; i < comboDriver->count(); i++ )
    {
        if ( fromGUI(comboDriver->text ( i ) ) == getDriver() )
            break;
    }
    if ( i < comboDriver->count() )
        comboDriver->setCurrentItem ( i );
    editDatabaseName->setText ( getDBName() );
    editHost->setText ( getHost() );
    editPort->setText ( Xtring::number ( getPort() ) );
    editUser->setText ( getUser() );
    editPassword->setText ( getPassword() );
    chkAutoLogin->setChecked ( isAutoLogin() );
    chkSavePassword->setChecked ( isSavePassword() );
    mLoginWelcome = Xtring::printf(_(loginWelcome),
                                   DBAPP->getPackageString().c_str() );
    if ( isAutoLogin() ) {
        setMessages (  mLoginWelcome, _("Intentando conectar automáticamente...") );
        accept();
        if ( mStatus == ok )
            return true;
    } else {
        setMessages ( mLoginWelcome, Xtring::printf(_(loginInstructions),
                      DBAPP->getPackageString().c_str() ) );
    }
    exec();
    return true;
}

void FrmLogin::updateConfig()
{
    gather();
    setAutoLogin ( chkAutoLogin->isChecked() );
    DBAPP->setUserLocalSetting ( "DBDRIVER", getDriver() );
    DBAPP->setUserLocalSetting ( "DBNAME", getDBName() );
    DBAPP->setUserLocalSetting ( "DBHOST", getHost() );
    DBAPP->setUserLocalSetting ( "DBPORT", getPort() );
    DBAPP->setUserLocalSetting ( "DBUSER", getUser() );
    DBAPP->setUserLocalSetting ( "DBSAVEPASSWORD", isSavePassword() );
    if ( isSavePassword() )
        DBAPP->setUserLocalSetting ( "DBPASSWORD", getPassword() );
    else
        DBAPP->setUserLocalSetting ( "DBPASSWORD", "" );
    DBAPP->setUserLocalSetting ( "AUTOLOGIN", isAutoLogin() );
    DBAPP->writeSettings();
}

void FrmLogin::gather()
{
    setDriver ( fromGUI(comboDriver->currentText()) );
    setDBName( editDatabaseName->toString() );
    setPort ( editPort->toInt() );
    setDBName ( editDatabaseName->toString() );
    setHost ( editHost->toString() );
    setUser ( editUser->toString() );
    setPassword ( editPassword->toString() );
// we don't want to change the original value of mupdate the config
// 	setAutoLogin ( chkAutoLogin->isChecked() );
    setSavePassword ( chkSavePassword->isChecked() );

    setCreateDBName ( editCreateDBName->toString() );
    setCreateDBDriver ( fromGUI(comboCreateDBDriver->currentText()) );
    setCreateDBPort ( editCreateDBPort->toInt() );
    setCreateDBName ( editCreateDBName->toString() );
    setCreateDBHost ( editCreateDBHost->toString() );
    setCreateDBUser ( editCreateDBUser->toString() );
    setCreateDBPassword ( editCreateDBPassword->toString() );
    setCreateUserUser ( editCreateUserUser->toString() );
    setCreateUserPassword ( editCreateUserPassword->toString() );
}

void FrmLogin::slotCreateDatabase()
{
    gather();
    comboCreateDBDriver->setCurrentText ( toGUI(getDriver()) );
    editCreateDBPort->setText ( Xtring::number ( getPort() ) );
    editCreateDBHost->setText ( getHost() );
    editCreateDBName->setText ( getDBName() );
    if ( getDriver() == "QPSQL7" )
        editCreateDBUser->setText ( "postgres" );
    else
        editCreateDBUser->setText ( "root" );
    editCreateDBPassword->setText ( "" );
    editCreateUserUser->setText ( getUser() );
    editCreateUserPassword->setText ( getPassword() );
    textCreateDBWelcome->setText ( toGUI(_( "Creando la base de datos de " ) + DBAPP->getPackageString() ) );
    textCreateDBDetailed->setText ( toGUI(_(
            "   Para crear la base de datos tienes que tener privilegios de SUPERUSUARIA"
            " en el servidor que hayas elegido. Rellena los datos del cuadro "
            "'Localización de la base de datos':\n"
            "   * TIPO DE SERVIDOR: MySQL\n"
            "   * PUERTO: 0 (normalmente)\n"
            "   * SERVIDOR: Dirección IP o nombre del servidor. Vacío si el servidor es este ordenador\n"
            "   * SUPERUSUARIA: Nombre de la usuaria con privilegios para crear bases de datos (root)\n"
            "   * CONTRASEÑA de la superusuaria.\n\n"
            "   Si no lo has hecho ya, rellena también el cuadro de la izquierda con"
            " el nombre de usuaria y contraseña de la usuaria que accederá usualmente a "
            "la base de datos que será creada." )) );
    // Por si pulsa el boton atras...
    setMessages ( mLoginWelcome, Xtring::printf(_(loginInstructions),
                  DBAPP->getPackageString().c_str() ) );

    pTabWidget->setTabEnabled( 1, true );
    pTabWidget->setCurrentIndex( 1 );
}

dbError FrmLogin::createDatabase()
{
    try {
        if( DBAPP->getConnection()->connect( dbConnection::SqlDriver(comboCreateDBDriver->currentIndex()),
                                             getCreateDBUser(), getCreateDBPassword(), "", getCreateDBHost(), getCreateDBPort()) ) {
            // TODO: Opción para crear las tablas sólamente
            bool forcecreatetables = DBAPP->getAppSetting("SYSTEM.FORCECREATEDBTABLES", false).toBool();
            if( !DBAPP->getConnection()->existsDatabase( mDBName ) ) {
                // Si el encoding está en blanco, se toma el de la instalación de MYSQL en el sistema.
                addMessage( Xtring::printf(_("Creando la base de datos %s ..."), mDBName.c_str()) );
                DBAPP->getDatabase()->setEncoding( "UTF8" );
                DBAPP->getDatabase()->setName( mDBName );
                DBAPP->getDatabase()->create( DBAPP->getConnection(),
                                              "DEFAULT COLLATE utf8_spanish_ci", false );
                forcecreatetables = true;
            }
            DBAPP->getConnection()->selectDatabase( mDBName );
            if( forcecreatetables == true ) {
                addMessage( _("Creando las tablas...") );
                DBAPP->getDatabase()->createTables( DBAPP->getConnection(), Xtring::null, false );
                DBAPP->getDatabase()->dropIndexes( DBAPP->getConnection(), false, true);
                DBAPP->getDatabase()->createIndexes( DBAPP->getConnection(), true );
            }
        }
    } catch( dbError e ) {
        _GONG_DEBUG_WARNING( e.what() );
    }
    return DBAPP->getConnection()->getLastError();
}


dbError FrmLogin::createUser()
{
    addMessage ( _("Creando usuario/a...") );
    try {
        DBAPP->getConnection()->createUser( mDBName,
                                            getRealUser( getCreateUserUser() ),
                                            getCreateUserPassword(), getCreateDBHost(), Xtring::null );
    } catch( dbError e ) {
        _GONG_DEBUG_WARNING( e.what() );
        return e;
    }
#ifdef TODO_LATER
    try
    {
        RecUser user( DBAPP->getConnection() );
        user.setValue("USER", getCreateUserUser() );
        user.setValue("PASSWORD", getCreateUserPassword() );
        setUser ( getCreateUserUser() );
        setPassword ( getCreateUserPassword() );
        user.save();
    } catch( dbError e ) {
        DBAPP->getConnection()->clearError();
    }
#endif
    return DBAPP->getConnection()->getLastError();
}

void FrmLogin::accept()
{
    static Xtring datosinc = _("Datos incompletos");
    gather();
    switch ( pTabWidget->currentIndex() )
    {
    case 0:   // Conexion
        if ( getUser().isEmpty() )
        {
            FrmBase::msgError( datosinc, _("Introduce un nombre de usuaria" ), Xtring::null, this );
            editUser->setFocus();
            return;
        }
        if( comboDriver->currentIndex() == 1 ) { // Postgresql
            FrmBase::msgError( datosinc, _("El driver POSTGRESQL no se puede utilizar aún" ), Xtring::null, this );
            comboDriver->setFocus();
            return;
        }
        setMessages ( mLoginWelcome, _("Conectando...") );
        if( getDBName().isEmpty() )
            mDBName = proposedDBName();
        editDatabaseName->setText ( mDBName );

        DBAPP->waitCursor();
        DBAPP->getConnection()->connect( dbConnection::SqlDriver(comboDriver->currentIndex()),
                                         getRealUser(getUser()),
                                         getPassword(), mDBName, getHost(), getPort() );
        DBAPP->resetCursor();
        break;
    case 1:   // Creacion de la base de datos
        if ( getCreateDBUser().isEmpty() )
        {
            FrmBase::msgError ( datosinc, _("Introduce un nombre de usuaria para crear la base de datos" ), Xtring::null, this );
            editCreateDBUser->setFocus();
            return;
        }
        if ( getCreateUserUser().isEmpty() )
        {
            FrmBase::msgError ( datosinc, _("Introduce un nombre de usuaria para conectar con la base de datos" ), Xtring::null, this );
            editCreateUserUser->setFocus();
            return;
        }
        if ( getCreateUserPassword().isEmpty() ) {
            if( FrmBase::msgYesNo ( this, _("La contraseña de usuaria está vacía.\nEs recomendable introducir una por razones de seguridad.\n\n¿Quieres continuar con la contraseña vacía?" ) ) == false ) {
                return;
            }
        }
        DBAPP->waitCursor();
        if( getCreateDBName().isEmpty() )
            mDBName = proposedDBName();
        else
            mDBName = getCreateDBName();
        setMessages ( mLoginWelcome, Xtring::printf(_("Creando la base de datos %s ..."), mDBName.c_str() ) );
        if( createDatabase().getNumber() == 0 ) {
            if( createUser().getNumber() != 0 ) {
                FrmBase::msgError( "Error creando usuaria", Xtring::printf(
                                       _("No se ha podido crear la usuaria '%s', por lo tanto, para acceder a la base de datos tendrás que utilizar "
                                         "la misma usuaria que la utilizada para crear la base de datos."), getCreateUserUser().c_str() ), Xtring::null, this );
                DBAPP->getConnection()->clearError();
            }
        }
        DBAPP->resetCursor();
        break;
    case 2:
    {
    }
    break;
    default:
        ;
    }

    if ( DBAPP->getConnection()->getLastError().getNumber() == 0 ) {
        /// La conexión se ha realizado con éxito y la base de datos existe
        addMessage( _("Conexión satisfactoria.") );
        mStatus = ok;
        if( mAutoLogin )
            setResult( QDialog::Accepted );
		QDialog::accept();
        updateConfig();
    }
    else
    {
        Xtring welcomeError, detailedError;
        pushCreateDatabase->setEnabled ( false );
        switch ( DBAPP->getConnection()->getLastError().getNumber() )
        {
        case 1044: // Access denied for user ..@.. to database xxx. The user and password is right, but the database never existed
        case 1045: // Access denied for user ..@.. to database xxx. The user or password is wrong.
            // It doesn't matter whether the database exists or not
        case 1049: // Unknown database ....., but the user and password are right
            detailedError = Xtring::printf( _("El usuario/a '%1s@%2s' no ha podido conectar con la base de datos '%3s' en %4s. "
                                              "Esto puede deberse a que la contraseña es errónea o bien a que la base de datos no existe.\n"
                                              "Comprueba el nombre de USUARIO/A y la CONTRASEÑA.\n"),
                                            pTabWidget->currentIndex() == 0 ? getUser().c_str() : getCreateDBUser().c_str(),
                                            pTabWidget->currentIndex() == 0 ? getHost().c_str() : getCreateDBHost().c_str(),
                                            mDBName.c_str(),
                                            getHost().isEmpty() ? _("este ordenador") : _("el servidor seleccionado") );
            pushCreateDatabase->setEnabled ( true );
            break;
        case 2002: // Localhost not running
            detailedError = Xtring::printf( _( "El servidor de bases de datos '%1s' "
                                               "no está instalado o funcionando en este ordenador.\n"
                                               "Prueba a iniciar el servidor de la base de datos o elige un servidor diferente "
                                               "y después pulsa de nuevo el botón 'Siguiente'.\n"), getDriver().c_str() );
            break;
        case 2003: // host not running
            detailedError = Xtring::printf( _( "El servidor de bases de datos '%1s' "
                                               "no está instalado o funcionando en el servidor '%2s'.\n"
                                               "Inicia el servidor de la base de datos o elige un servidor diferente "
                                               "y después pulsa de nuevo el botón 'Siguiente'.\n"),
                                            getDriver().c_str(), getHost().c_str());
            break;
        case 2005: // GDBERR_HOST_NOT_FOUND:
            detailedError = Xtring::printf( _( "No se ha podido acceder al servidor de bases de datos.\n"
                                               "O bien el ordenador '%1s' es inaccesible o el servidor de bases de datos '%2s' "
                                               "no está funcionando en ese ordenador.\n"
                                               "Verifica que el ordenador '%3s' es accesible y que el servidor "
                                               "de la base de datos %4s está funcionando y después pulsa de nuevo el botón 'Siguiente'."),
                                            getHost().c_str(),
                                            getDriver().c_str(),
                                            getHost().c_str(),
                                            getDriver().c_str());
            break;
#if 0
        case GDBERR_DRIVER_NOT_FOUND:
        {
            detailedError = ("El driver '%1' no se ha podido cargar.\n"
                             "Los drivers disponibles son:\n" ).arg ( getDriver() );
            XtringList list = GSqlDatabase::database() ->drivers();
            for ( XtringList::Iterator it = list.begin(); it != list.end(); ++it )
                detailedError += "\t" + *it + "\n";
        }
        break;
#endif

        default:
            _GONG_DEBUG_WARNING ( Xtring::printf ( "Error %d",
                                                   DBAPP->getConnection()->getLastError().getNumber() ) );
            break;
        }
        Xtring servererror = Xtring::printf( _( "\n\nError del servidor SQL:\n%1d: %2s\n"),
                                             DBAPP->getConnection()->getLastError().getNumber(),
                                             DBAPP->getConnection()->getLastError().what());
        switch ( pTabWidget->currentIndex() )
        {
        case 0:
            textWelcome->setText ( toGUI(_( "Error de conexión con la base de datos\n" )) );
            if( pushCreateDatabase->isEnabled() )
                detailedError += _("Si la base de datos no está creada, pulsa el botón 'CREAR LA BASE DE DATOS'" );
            textDetailed->setText ( toGUI(detailedError + servererror) );
            break;
        case 1:
            textCreateDBWelcome->setText ( toGUI( _("Error creando la base de datos" ) ) );
            if( DBAPP->getConnection()->getLastError().getNumber() != 2002 ) {
                detailedError = _(  "Los datos para la creación de la base de datos no son correctos.\n"
                                    "Revisa el nombre de SUPERUSUARIA, la CONTRASEÑA y el nombre y tipo del servidor "
                                    "de la base de datos.\n"
                                    "Recuerda que si la base de datos no está en este ordenador, debes rellenar "
                                    "el nombre del servidor. Si la base de datos está en este ordenador, "
                                    "deja el nombre en blanco.\n\n") + detailedError;
            }
            textCreateDBDetailed->setText ( toGUI(detailedError + servererror) );
            break;
        }
    }
}

void FrmLogin::setMessages ( const Xtring &welcome, const Xtring &detailed )
{
    switch ( pTabWidget->currentIndex() )
    {
    case 0:
        textWelcome->setText ( toGUI(welcome) );
        textDetailed->setText ( detailed );
        break;
    case 1:
        textCreateDBWelcome->setText ( toGUI(welcome) );
        textCreateDBDetailed->setText ( detailed );
        break;
    }
}


void FrmLogin::addMessage ( const Xtring &message )
{
    switch ( pTabWidget->currentIndex() )
    {
    case 0:
        textDetailed->setText ( toGUI(textDetailed->toString() + "\n" + message) );
        break;
    case 1:
        textCreateDBDetailed->setText ( toGUI(textCreateDBDetailed->toString() + "\n" + message) );
        break;
    }
}

/**
 * Elimina el número de asociación del nombre de la base de datos, para volver a hacer login
 * @param composedname
 * @return
 */
Xtring FrmLogin::getDBBaseName( const Xtring &composedname ) const
{
    Xtring r4 = composedname.right(4);
    bool ok = false;
    r4.toInt(&ok);
    if( ok )
        return composedname.left( composedname.length() - 4 );
    else
        return composedname;
}

Xtring FrmLogin::proposedDBName() const
{
    return getDBBaseName(DBAPP->getDatabase()->getName());
//		+ Xtring::number ( codasoc ).padL ( 4, '0' );
}

} // namespace gong
