#include <gongfileutils.h>
#include <gongdbdefinition.h>
#include <gongdbmastertable.h>
#include <gongdbsettings.h>
#include <gonggettext.h>
#include <gongdbrecordlist.h>
#include <gongdbrecordpermissions.h>
#include "dbappmainwindow.h"
#include "dbappdbapplication.h"
#include "dbappfrmlogin.h"
#include "dbappdbmodule.h"
#include "dbappfrmdatabasetools.h"
#include "dbapprecmetadbdata.h"
#include "dbappnameslisttable.h"

/// @TODO borrar con el sleep
#include "unistd.h"

namespace gong {

dbApplication *DBAPP = 0;
// static members
Xtring dbApplication::sCodeNotFound, dbApplication::sDescNotFound;
List<dbRecordID> dbApplication::sSeekCodeRecordIDs;
Date dbApplication::sWorkingDate = Date::currentDate();
dbRecordID dbApplication::sAnotherRecordID = 0;
XtringList dbApplication::mMasterTables, dbApplication::mDetailTables;
dbRecordTimestampBehavior *dbApplication::sTimestampBehavior
    = new dbRecordTimestampBehavior( "REC_FECHA_CREA", "REC_FECHA_MODIF" );

static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::Bool,
        "AUTOLOGIN",
        _("Conectar automáticamente a la base de datos al iniciar el programa"),
        "false",
        dbModuleSetting::Local
    },
// TODO
//     {
//         dbModuleSetting::Bool,
//         "SAVE_WINDOW_GEOMETRY",
//         _("Recordar tamaños y posiciones de las ventanas"),
//         "true"
//     },
    // INFORMES
    {
        dbModuleSetting::DirName,
        "RTK.PATH",
        _("Ruta de búsqueda de informes"),
        "",
        dbModuleSetting::Local
    },
    {
        dbModuleSetting::Int,
        "RTK.OUTPUT.PRINTER.MARGINLEFT",
        _("Margen izquierdo de los informes en impresora y pantalla"),
        "20",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::Int,
        "RTK.OUTPUT.PRINTER.MARGINRIGHT",
        _("Margen derecho de los informes en impresora y pantalla"),
        "20",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::Int,
        "RTK.OUTPUT.PRINTER.MARGINTOP",
        _("Margen superior de los informes en impresora y pantalla"),
        "20",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::Int,
        "RTK.OUTPUT.PRINTER.MARGININFERIOR",
        _("Margen inferior de los informes en impresora y pantalla"),
        "20",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::Int,
        "RTK.OUTPUT.PDF.MARGINLEFT",
        _("Margen izquierdo de los informes en PDF"),
        "20",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::Int,
        "RTK.OUTPUT.PDF.MARGINRIGHT",
        _("Margen derecho de los informes en PDF"),
        "20",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::Int,
        "RTK.OUTPUT.PDF.MARGINTOP",
        _("Margen superior de los informes en PDF"),
        "20",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::Int,
        "RTK.OUTPUT.PDF.MARGININFERIOR",
        _("Margen inferior de los informes en PDF"),
        "20",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::String,
        "SMTP_FROM",
        _("Remitente de los envíos de emails masivos"),
        "",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::String,
        "SMTP_HOST",
        _("Host del servidor SMTP"),
        "",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::String,
        "SMTP_USER",
        _("Usuaria del servidor SMTP"),
        "",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::Int,
        "SMTP_PORT",
        _("Puerto del servidor SMTP (25)"),
        "25",
        dbModuleSetting::All
    },
    {
        dbModuleSetting::String,
        "ICON_THEME",
        _("Tema de los iconos"),
        "",
        dbModuleSetting::All
    },
    {dbModuleSetting::None}
};

dbApplication::dbApplication ( const char *dbversion, const char *datadir,
                               const char *packagename, const char *packageversion,
                               int &argc, char **argv )
    : GuiApplication ( datadir, packagename, packageversion, argc, argv ),
      pConnection( new dbConnection() ), pDatabase ( 0 ), pFrmLogin( 0 ),
      mdbVersion( dbversion ), pGlobalSettings(0), pUserGlobalSettings(0),
      mDatabaseChanged( false ), mReadOnly(false), mRealSQLUser( false )
{
    DBAPP = this;
    _GONG_DEBUG_TRACE ( 1 );
    pMachineSettings = new Settings( Xtring::null, _("Configuración por omisión de este ordenador") );
    pMachineSettings->setReadOnly( true );
    // We need some basic settings in order to log in (databse settings )
    pMachineSettings->read( getGonglibDataDir() + "dbapp/settings.rc" );
    pUserLocalSettings = new Settings( getLocalDataDir() + "settings.rc",
                                       _("Configuración local para este ordenador") );
    pUserLocalSettings->read(); // will be read again later
    pModuleSettings = _settings;
}

dbApplication::~dbApplication()
{
    delete pMachineSettings;
    delete pUserLocalSettings;
    if( pGlobalSettings )
        delete pGlobalSettings;
    if( pUserGlobalSettings )
        delete pUserGlobalSettings;
    if ( pConnection )
        delete pConnection;
    if ( pDatabase )
        delete pDatabase;
}

RecMetaDBData* dbApplication::getRecMetaDBData() const
{
    _GONG_DEBUG_ASSERT( pRecMetaDBData );
    return pRecMetaDBData;
}

/**
 * @brief ...
 * http://qt.developpez.com/doc/4.6/stylesheet-reference/
 * @return void
 **/
void dbApplication::readSettings()
{
    _GONG_DEBUG_PRINT(1, "Reading settings for the application" );
    // Local machine settings are read before everything else
    QString ss( styleSheet() );
    bool ssread = false;
    if( FileUtils::exists( (getGonglibDataDir() + "dbapp/stylesheet.css").c_str() ) ) {
        ss += FileUtils::readFile( getGonglibDataDir() + "dbapp/stylesheet.css" ).c_str();
        ssread = true;
        _GONG_DEBUG_PRINT(2, "Adding stylesheet: " + getGonglibDataDir() + "dbapp/stylesheet.css" );
    } else {
        _GONG_DEBUG_PRINT(3, "NOT ADDING stylesheet: " + getGonglibDataDir() + "dbapp/stylesheet.css" );
    }
    if( ssread )
        setStyleSheet( ss );
    ssread = false;
    mReportsGlobalPath = getGonglibDataDir() + "dbapp/informes/";
    // Now, each module's global and local settings
    for ( unsigned int i=0; i < mModules.size(); i++ ) {
        dbModule *mod = mModules.seq_at(i);
        if( mod->isEnabled() )
            mod->readSettings();
    }
    // Each module has added its reports path in readSettings
    mReportsGlobalPath += ":" + getGlobalDataDir() + "informes/";
    if( !mReportsLocalPath.endsWith(":") )
        mReportsLocalPath += ":";
    mReportsLocalPath += getLocalDataDir() + "informes/";
    // dbModules might have set the stylesheet, so I read it again
    ss = styleSheet();
    if( FileUtils::exists( (getGlobalDataDir() + "stylesheet.css").c_str() ) ) {
        ss += FileUtils::readFile( getGlobalDataDir() + "stylesheet.css" ).c_str();
        ssread = true;
        _GONG_DEBUG_PRINT(2, "Adding stylesheet: " + getGlobalDataDir() + "stylesheet.css" );
    } else {
        _GONG_DEBUG_PRINT(3, "NOT ADDING stylesheet: " + getGlobalDataDir() + "stylesheet.css" );
    }
    if( FileUtils::exists( (getLocalDataDir() + "stylesheet.css").c_str() ) ) {
        ss += FileUtils::readFile( getLocalDataDir() + "stylesheet.css" ).c_str();
        ssread = true;
        _GONG_DEBUG_PRINT(2, "Adding stylesheet: " + getLocalDataDir() + "stylesheet.css" );
    } else {
        _GONG_DEBUG_PRINT(3, "NOT ADDING stylesheet: " + getLocalDataDir() + "stylesheet.css" );
    }
    if( ssread )
        setStyleSheet( ss );
    // The package settings take precedence over the modules settings
    pMachineSettings->read( getGlobalDataDir() + "settings.rc" );
    pMachineSettings->read( getGlobalDataDir() + Xtring(getPackageName()).lower() + ".ddd" );
    // Ŕead again the userlocalsettings so that they take precedence over the modules' settings
    pUserLocalSettings->read();
    _GONG_DEBUG_PRINT(1, QIcon::themeName().latin1() );
    const char *tn = getAppSetting( "ICON_THEME" ).toString().c_str();
    if( !strempty(tn) && tn != QIcon::themeName() )
        QIcon::setThemeName( tn );

}

bool dbApplication::readDatabaseSettings(const Xtring& tablename, const Xtring& filter)
{
    if( !pGlobalSettings )
        pGlobalSettings = new dbSettings( getConnection(),
                                          tablename, "NAME", "VALUE", "TYPE", "USER_ID", 0,
                                          _("Configuración global") );
    else
        pGlobalSettings->clear();
    return pGlobalSettings->read();
// 	pUserGlobalSettings = new dbSettings( getConnection(),
// 	                                 "USER_GLOBAL_CONFIG", "NAME", "VALUE", "USER", "SCOPE", "USER",
// 	                                 _("Configuración global de la usuaria") );
// 	pUserGlobalSettings->read();
}


/**
 * @brief gets all the applications reports paths separated by :
 *
 * @param reversed if true, reverse the paths
 * @return Xtring
 **/
Xtring dbApplication::getReportsPath( bool reversed )
{
    Xtring ret = mReportsGlobalPath + ":" + mReportsLocalPath + ":" + DBAPP->getAppSettingConcat( "RTK.PATH", ":", true );
    if( reversed ) {
        XtringList paths;
        ret.tokenize( paths, ":");
        ret.clear();
        for( XtringList::reverse_iterator it = paths.rbegin(); it != paths.rend(); ++it ) {
            if( !ret.empty() )
                ret += ":";
            ret += *it;
        }
    }
    return ret;
}

/**
 * @brief Adds a path to the application's reports path
 *        Paths are added when reading each \a dbModule settings
 * @param local whether is a local path
 * @param path ...
 * @return void
 **/
void dbApplication::addReportsPath(bool local, const Xtring& path)
{
    if( path.isEmpty() )
        return;
    if( local ) {
        if( !mReportsLocalPath.isEmpty() )
            mReportsLocalPath += ":";
        mReportsLocalPath += path;
    } else {
        if( !mReportsGlobalPath.isEmpty() )
            mReportsGlobalPath += ":";
        mReportsGlobalPath += path;
    }
}

/**
 * @brief Logs in the application and logs in each module
 *
 * @param version ...
 * @param startingapp if false, we are login from inside the application
 * @param autologin try to log in without showing the form
 * @return bool
 **/
bool dbApplication::login( const Xtring &version, bool startingapp, bool autologin )
{
    bool bAutoLogin;
    if( startingapp ) {
        bAutoLogin = getAppSetting("AUTOLOGIN", false).toBool();
    } else
        bAutoLogin = autologin;
    pFrmLogin = new FrmLogin(startingapp);
    pFrmLogin->setAutoLogin(bAutoLogin);
    pFrmLogin->init(); // Tratar de hacer el autologin
    if( pFrmLogin->result() == QDialog::Rejected ) {
        delete pFrmLogin;
        pFrmLogin = 0;
        return false;
    }
    mDbUser = pFrmLogin->getUser();
    mDbHost = pFrmLogin->getHost();
    mDbUserPassword = pFrmLogin->getPassword();
    getConnection()->selectDatabase( pFrmLogin->getDBName() );
    getDatabase()->setName( pFrmLogin->getDBName() );

    if( startingapp ) {
        // Create some needed tables, after logging in to the database
        if( !getConnection()->existsTable( "METADBDATA" ) ) {
            pFicMetaDBData->getTableDefinition()->create( getConnection(), true);
            pFicMetaDBData->getTableDefinition()->createIndexes( getConnection());
        }
        if( !getConnection()->existsTable( "GLOBALCONFIG" ) ) {
            pFicGlobalConfig->getTableDefinition()->create( getConnection(), true);
            pFicGlobalConfig->getTableDefinition()->createIndexes( getConnection(), false);
        }

        // Check the main program version against the database version
        Xtring versionInDatabase = getdbVersion();
        try {
            versionInDatabase = getConnection()->selectString( "SELECT VERSION FROM METADBDATA WHERE MODULENAME='gong'" );
        } catch( ... ) {
        }
        long nversionindatabase = version2Long( versionInDatabase );
        long nversioninprogram = version2Long( getdbVersion() );
        if ( nversionindatabase > nversioninprogram ) {
            FrmBase::msgError( DBAPP->getPackageString(),
                               Xtring::printf( _("%s no está actualizado.\n"
                                                 "La base de datos tiene la versión '%s(%d)' y este programa está preparado para "
                                                 "utilizar la versión '%s(%d)'\n"
                                                 "Actualiza el programa.\n"),
                                               DBAPP->getPackageString().c_str(), versionInDatabase.c_str(), nversionindatabase,
                                               DBAPP->getdbVersion().c_str(), nversioninprogram ) );
            return false;
        }

        if( !pRecMetaDBData )
            pRecMetaDBData = new RecMetaDBData( getConnection() );
        if( !pRecMetaDBData->read( "MODULENAME='gong'" ) ) {
            pRecMetaDBData->setValue( "MODULENAME", "gong" );
            pRecMetaDBData->setValue( "ACTIVE", true );
            pRecMetaDBData->setValue( "ORDER", 0 );
            pRecMetaDBData->setValue( "LOCKED", false );
            pRecMetaDBData->setValue( "VERSION", getdbVersion() );
            pRecMetaDBData->save( false );
        }
        // Check if the database is locked
        if( pRecMetaDBData->getValue( "LOCKED" ).toBool() )
            mReadOnly = true;
    }

    // Read all the database stored settings
    DBAPP->readDatabaseSettings( "GLOBALCONFIG", Xtring::null );

    // Do the login of the plugins and add their title to the application title
    mTitle = getPackageString();
    for ( unsigned int i=0; i < mModules.size(); i++ ) {
        dbModule *mod = mModules.seq_at(i);
        if( mod->isEnabled() ) {
            Xtring title;
            _GONG_DEBUG_PRINT(2, "Module " + mod->getUnixName() + " logging in" );
            if ( !mod->login ( pFrmLogin, version, title, startingapp ) ) {
                _GONG_DEBUG_WARNING( "Module " + mod->getUnixName() + " has not logged in" );
                mod->setEnabled( false );
            } else {
                if( !title.isEmpty() )
                    mTitle += " " + title;
            }
        }
    }

    if( getRecMetaDBData()->getValue( "LOCKED" ).toBool() ) {
        FrmBase::msgOk( getPackageString(),
                        Xtring::printf("La base de datos fue bloqueada el día %s por la usuaria %s:"
                                       "\n%s\nPara desbloquearla, ve a Sistema->Herramientas de la base de datos",
                                       getRecMetaDBData()->getValue( "LOCKED_DATE" ).toString().c_str(),
                                       getRecMetaDBData()->getValue( "LOCKED_USER" ).toString().c_str(),
                                       getRecMetaDBData()->getValue( "LOCKED_DESC" ).toString().c_str() ),
                        FrmBase::information );
    } else {
        // Do a backup if scheduled to
        int backup_interval = getAppSetting( "SYSTEM.DBBACKUP.INTERVAL" ).toInt();
        if(  backup_interval > 0 ) {
            Date last_backup = getAppSetting( "SYSTEM.DBBACKUP.LAST_BACKUP",
                                              (Date::currentDate() -3).toString("%Y-%m-%d") ).toDate();
            if( Date::currentDate() - last_backup > backup_interval ) {
                showStickyOSD( getPackageString(),
                               Xtring::printf("Hace %d días que no se ha realizado una copia de seguridad",
                                              Date::currentDate() - last_backup) );
                FrmDatabaseTools *frmdbtools = new FrmDatabaseTools();
                frmdbtools->backupDatabase( getAppSetting( "SYSTEM.DBBACKUP.AUTOMATIC" ).toBool() );
                delete frmdbtools;
            }
        }
    }

    _GONG_DEBUG_PRINT(0, getRecMetaDBData()->getValue( "LOCKED" ).toBool() );

    // Check if the database has changed
    if( startingapp ) {
        pFrmLogin->addMessage( _("Comprobando las versiones de los módulos...") );
        DBAPP->processEvents();

        // Check the modules database versions against the modules program versions
        RecMetaDBData *module = new RecMetaDBData( getConnection() );
        _GONG_DEBUG_ASSERT( module );
        _GONG_DEBUG_ASSERT( module->getConnection() );
        for ( unsigned int i=0; i < mModules.size(); i++ ) {
            dbModule *mod = mModules.seq_at(i);
            if( !mod->isEnabled() )
                continue;
            Xtring mod_name = mod->getUnixName().upper();
            uint mod_version = mod->getVersion();
            if( !module->read( "MODULENAME=" + module->getConnection()->toSQL( mod_name ) ) ) {
                module->setValue( "MODULENAME", mod_name );
                module->setValue( "ACTIVE", true );
                module->setValue( "ORDER", 10 * (i+1) );
                module->setValue( "LOCKED", false );
                module->setValue( "VERSION", Xtring::number(mod_version) );
                module->save( false );
            }
            uint db_mod_version = module->getValue( "VERSION" ).toInt();
            if( mod_version != 0 ) {
                _GONG_DEBUG_PRINT(1, "Module " + mod->getUnixName() + ":version: " + Xtring::number( mod_version ) + ", dbversion " + Xtring::number( db_mod_version ) );
                if( mod_version < db_mod_version ) {
                    FrmBase::msgError( DBAPP->getPackageString(),
                                       Xtring::printf( _("%s no está actualizado.\n"
                                                         "El módulo %s tiene la versión de base de datos '%d' y este programa está preparado para "
                                                         "utilizar la versión '%d'\n"
                                                         "Actualiza el programa.\n"),
                                                       DBAPP->getPackageString().c_str(),
                                                       mod->getUnixName().c_str(), db_mod_version, mod_version ) );
                    return false;
                } else if( mod_version > db_mod_version ) {
                    if( !mReadOnly ) {
                        for( uint v = db_mod_version; v < mod_version; ++v ) {
                            Xtring mig = mod->getMigrationSQL( v );
                            if( !mig.isEmpty() ) {
                                if( FrmBase::msgYesNoLarge(
                                            DBAPP->getPackageString(),
                                            Xtring::printf( _("La base de datos del módulo %s necesita actualizarse a la versión %d.\n¿Quieres actualizar?"),
                                                            mod_name.c_str(), mod_version ),
                                            Xtring(mig).replace(";", "\n") ) )
                                {
                                    XtringList querys;
                                    mig.tokenize( querys, ";" );
                                    try {
                                        pFrmLogin->addMessage( Xtring::printf(
                                                                   _("Actualizando módulo %s a la versión %d"), mod_name.c_str(), mod_version ) );

                                        DBAPP->processEvents();
                                        pConnection->exec( querys, false /* do not ignore errors */ );
                                        setDatabaseChanged( true );
                                        module->setValue( "VERSION", mod_version );
                                        module->save( false );
                                    } catch( dbError &e) {
                                        throw std::runtime_error(e.what());
                                    }
                                } else {
                                    quit();
                                }
                            }
                        }
                    }
                }
            }
        }
        delete module;

        pFrmLogin->addMessage( Xtring::printf(_("Comprobando la base de datos %s ..."),
                                              pFrmLogin->getDBName().c_str() ) );
        DBAPP->processEvents();
        Xtring diff = upgradeDatabase( getConnection(), getDatabase(),
                                       pFrmLogin->getDBName(), false /*not purging*/ );
        if( !diff.isEmpty() ) {
            pFrmLogin->addMessage( Xtring::printf( _("La base de datos de %s no está actualizada\n"),
                                                   getPackageString().c_str() ) );
            pFrmLogin->addMessage( diff );
            DBAPP->processEvents();
        }
    }

    if ( isDatabaseChanged() )
    {
        _GONG_DEBUG_WARNING( "Database has changed" );
        getDatabase()->dropIndexes ( getConnection(), false, true );
        getDatabase()->createIndexes ( getConnection(), true );
    }


    // Postload tasks for every module.
    for ( unsigned int i=0; i < mModules.size(); i++ ) {
        dbModule *mod = mModules.seq_at(i);
        if( mod->isEnabled() )
            mod->afterLoad();
    }

    /// after all the settings have been read and the database upgraded, configure the database
    setDDDFromConfig( pDatabase );
    setViewsFromConfig( pDatabase );
    setStylesFromConfig( pDatabase );
    NamesListTable::fillInfoList( getConnection() );

    if( !startingapp ) {
        delete pFrmLogin;
        pFrmLogin = 0;
    } // else it will be deleted in initMainWindow
    return getConnection()->isMySQL() || getConnection()->isPGSQL();
}

bool dbApplication::initDatabases()
{
    _GONG_DEBUG_TRACE ( 1 );
    pDatabase = new dbDefinition ( "gongdb", "Base de datos de GongLib" );

    pFicMetaDBData = new dbMasterTable( *pDatabase, "METADBDATA" );
    pFicMetaDBData->addFieldRecordID();
    pFicMetaDBData->addFieldStringCode("MODULENAME",100);
    pFicMetaDBData->addFieldString( "VERSION", 25 );
    pFicMetaDBData->addFieldBool("ACTIVE");
    pFicMetaDBData->addFieldInt("ORDER");
    pFicMetaDBData->addFieldBool( "LOCKED" );
    pFicMetaDBData->addFieldString( "LOCKED_DESC", 100 );
    pFicMetaDBData->addFieldString( "LOCKED_USER", 25 );
    pFicMetaDBData->addFieldDate( "LOCKED_DATE" );
    pFicMetaDBData->addFieldString( "LOCKED_PASSWORD", 25 );
    pFicMetaDBData->addBehavior( getRecordTimestampBehavior() );
    pDatabase->addTable( pFicMetaDBData->getTableDefinition() );

    pFicGlobalConfig = new dbMasterTable( *pDatabase, "GLOBALCONFIG" );
    pFicGlobalConfig->addFieldStringCode("NAME",100)->setUnique( false );
    pFicGlobalConfig->addFieldText("VALUE");
    pFicGlobalConfig->addFieldReferenceID("USER_ID", "USER.ID");
    pFicGlobalConfig->addFieldString("TYPE", 20);
    pDatabase->addTable( pFicGlobalConfig->getTableDefinition() );

    for ( unsigned int i=0; i<mModules.size(); i++ ) {
        dbModule *mod = mModules.seq_at(i);
        if( mod->isEnabled() ) {
            mod->initDatabase ( pDatabase );
            mMasterTables << mod->getMasterTables();
            mDetailTables << mod->getDetailTables();
        }
    }
    // This is done again later, but we do this now to show correct table names while login
    setDDDFromConfig( pDatabase );
    return true;
}

bool dbApplication::manageDbError(const Xtring& context, const dbError& e)
{
    if( e.getNumber() == 2006 ) {
        if( FrmBase::msgYesNo( _("Se ha perdido la conexión con la base de datos"),
                               _("¿Quieres intentar conectar de nuevo?") ) )
            return login( getPackageVersion(), false, true );
    }
    return false;
}


/**
 * Puesto que el login se hace antes que el initMainWindow, los logins de los plugins
 * no pueden poner el título de la ventana en ese momento, así que se va guardando el
 * título en la variable mTitle para ponerlo cuando se crea la ventana.
 * Esto es necesario también cuando se hace login dentro de la aplicación,
 * @return
 */
bool dbApplication::initMainWindow()
{
    pMainWindow = new MainWindow();
    pMainWindow->initGUI();
    // Si no, la aplicación no termina
    setMainWidget ( pMainWindow );
    bool ret = true;
    for ( unsigned int i=0; i<mModules.size(); i++ ) {
        dbModule *mod = mModules.seq_at(i);
        if( mod->isEnabled() )
            ret &= mod->initMainWindow ( static_cast<MainWindow *>(pMainWindow) );
    }
    pMainWindow->finishGUI();
    getMainWindow()->setWindowTitle( toGUI( mTitle ) );
    if( pFrmLogin ) {
        delete pFrmLogin;
        pFrmLogin = 0;
    }
    return ret;
}

/**
 * @brief ...
 *
 * @param parentfrm The parent FrmEditRec that will be disabled when this form is shown
 * @param rec ...
 * @param dm ...
 * @param editmode ...
 * @param editflags ...
 * @param parent The widget to which this form belongs to (QFrame, QTabWidget, ... )
 * @param name ...
 * @param fl ...
 * @return :FrmEditRec*
 **/
FrmEditRec *dbApplication::createEditForm ( FrmEditRec *parentfrm,
        dbRecord *rec, dbRecordDataModel *dm, DataTable::EditMode editmode,
        dbApplication::EditFlags editflags, QWidget *parent, const char* name,
        WidgetFlags fl )
{
    // Los formularios se buscan desde los últimos plugins cargados hasta los primeros
    FrmEditRec *frm = 0;
    int i;
    for( i = mModules.size()-1; i>=0; i-- ) {
        dbModule *mod = mModules.seq_at(i);
        if( !mod->isEnabled() )
            continue;
        frm = mod->createEditForm ( parentfrm, rec, dm, editmode, editflags, parent, name, fl );
        if ( frm )
            break;
    }
    if( frm ) {
        for	( i = 0; i < (int)mModules.size(); ++i ) {
            dbModule *mod = mModules.seq_at(i);
            if( !mod->isEnabled() )
                continue;
            _GONG_DEBUG_PRINT(4, "Calling afterCreateEditForm on module " + mod->getUnixName() + " for record " + rec->getTableName() );
            mod->afterCreateEditForm( frm, rec );
        }
    } else {
        _GONG_DEBUG_WARNING ( "No se ha podido crear un formulario para la tabla " + rec->getTableName() );
    }
    return frm;
}

FrmEditRecDetail *dbApplication::createEditDetailForm (
    FrmEditRecMaster *frmmaster, int ndetalle,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    DataTable::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    // Los formularios se buscan desde los ltimos plugins cargados hasta los primeros
    FrmEditRecDetail *frm = 0;
    int i;
    for( i = mModules.size()-1; i>=0; i-- ) {
        dbModule *mod = mModules.seq_at(i);
        if( !mod->isEnabled() )
            continue;
        frm = mod->createEditDetailForm ( frmmaster, ndetalle, rec, dettablename,
                                          dm, editmode, editflags, parent, name, fl );
        if ( frm )
            break;
    }
    if( frm ) {
        for	( i = 0; i < (int)mModules.size(); ++i ) {
            dbModule *mod = mModules.seq_at(i);
            if( !mod->isEnabled() )
                continue;
            mod->afterCreateEditForm( frm, rec );
        }
    } else {
        _GONG_DEBUG_WARNING ( "No se ha podido crear un formulario para la tabla " + rec->getTableName() );
    }
    return frm;
}

/**
 * @brief In order to use the gonglib with databases, you have to define this function
 *
 * @return dbRecord*
 **/
dbRecord* dbRecord::duplicate()
{
    return DBAPP->createRecord( getTableName(), 0, getUser() );
}

dbRecord *dbApplication::createRecord( const Xtring &tablename, dbRecordID recid, dbUser *user )
{
    _GONG_DEBUG_ASSERT ( getConnection() );
    _GONG_DEBUG_ASSERT ( getDatabase() );
    // Los registros se buscan desde los últimos plugins cargados hasta los primeros
// 	_GONG_DEBUG_PRINT ( 5, Xtring::printf ( "(%s,%d)", tablename.c_str(), recid ) );
    dbRecord *rec = 0;
    int i;
    for ( i = mModules.size()-1; i>=0; i-- ) {
        dbModule *mod = mModules.seq_at(i);
        if( !mod->isEnabled() )
            continue;
        try
        {
            rec = mod->createRecord ( tablename, recid, user );
        }
        catch ( std::runtime_error &e )
        {
            _GONG_DEBUG_WARNING ( e.what() );
        }
        if ( rec ) {
            rec->clear( false ); // Do not set custom default values
            rec->init();
            break;
        }
    }
    if( rec ) {
        for ( i = mModules.size()-1; i>=0; i-- ) {
            dbModule *mod = mModules.seq_at(i);
            if( mod->isEnabled() )
                mod->afterCreateRecord( rec );
        }
    } else {
        class DerivedRecord: public dbRecord {
        public:
            DerivedRecord(gong::dbConnection *conn, gong::dbTableDefinition *tbldef, gong::dbRecordID recid=0, gong::dbUser *user=0)
                :dbRecord(conn, tbldef, recid, user) {}
        };
        DerivedRecord *rec = new DerivedRecord( getConnection(),
                                                getDatabase()->findTableDefinition(tablename),
                                                recid, user );
        _GONG_DEBUG_WARNING ( "Creating a default record for the table " + tablename );
        return rec;
    }
    return rec;
}

dbRecord *dbApplication::createRecord ( const dbRecord *other )
{
    return createRecord( other->getTableName(), other->getRecordID(), other->getUser() );
}


bool dbApplication::editRecord( FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
                                DataTable::EditMode editmode, dbApplication::EditFlags editflags,
                                QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT ( rec );
    bool ret = false;
    FrmEditRec *editform = createEditForm ( parentfrm, rec, dm, editmode,
                                            static_cast<dbApplication::EditFlags>(editflags | dbApplication::simpleEdition),
                                            theGuiApp->getMainWindow() ? theGuiApp->getMainWindow()->getViewport() : 0,
                                            ( name?name:Xtring ( "edit_Rec" + rec->getTableName() ).c_str() ), fl );
    if( editform ) {
        editform->showModalFor( parentfrm, false, true );
        ret = editform->isSaved();
        delete editform;
    }
    return ret;
}

FrmEditRec *dbApplication::browse ( FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
                                    DataTable::EditMode /*editmode*/, dbApplication::EditFlags editflags,
                                    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT ( rec );
    FrmEditRec *editform;
    editform = createEditForm ( parentfrm, rec, dm, DataTable::browsing,
                                //static_cast<dbApplication::EditFlags> ( editflags | dbApplication::editContinuous ),
                                editflags,
                                parent, ( name?name:Xtring ( "browse_Rec" + rec->getTableName() ).c_str() ), fl );
    _GONG_DEBUG_ASSERT ( editform );
    return editform;
}

dbRecordID dbApplication::choose( FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
                                  dbApplication::EditFlags editflags,
                                  QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT ( rec );
    QWidget *fw = focusWidget();
    FrmEditRec *chooseform;
    dbRecordID ret = 0;
    chooseform = createEditForm ( parentfrm, rec, dm, DataTable::choosing, editflags,
                                  theGuiApp->getMainWindow() ? theGuiApp->getMainWindow()->getViewport() : 0,
                                  ( name?name:Xtring ( "choose_Rec" + rec->getTableName() ).c_str() ), fl );
    if( chooseform ) {
        chooseform->showModalFor( parent, true, true );
        if( !chooseform->wasCancelled() )
            ret = chooseform->getRecordID();
        delete chooseform;
    } else {
        FrmBase::msgError( getPackageName(),
                           Xtring::printf( _("No se ha podido crear un formulario para la tabla de %s"),
                                           rec->getTableDefinition()->getDescPlural().c_str() ) );
    }
    if ( fw && fw != focusWidget() )
    {
        // Si el foco lo había tomado un LineEdit, evitar que se llame a su evento validate
        if ( LineEdit *bogusfocuswidget = dynamic_cast<LineEdit *> ( focusWidget() ) )
            bogusfocuswidget->setCancelling();
    }
    return ret;
}

bool dbApplication::chooseMulti( List<dbRecordID> &v,
                                 FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
                                 dbApplication::EditFlags editflags,
                                 QWidget *parent, const char* name, WidgetFlags fl )
{
    bool ret = false;
    _GONG_DEBUG_ASSERT ( rec );
    FrmEditRecMaster *choosemaster = static_cast<FrmEditRecMaster *>
                                     ( createEditForm ( parentfrm, rec, dm, DataTable::choosing, editflags, 0,
                                             ( name?name:Xtring ( "choose_Rec" + rec->getTableName() ).c_str() ), fl ) );
    if( !choosemaster ) {
        choosemaster = new FrmEditRecMaster( parentfrm, rec, dm, DataTable::choosing,
                                             dbApplication::EditFlags( editflags | dbApplication::readOnly | dbApplication::onlyBrowse ), 0,
                                             ( name?name:Xtring ( "onlyBrowse_Rec" + rec->getTableName() ).c_str() ), fl );
        choosemaster->setMinimumWidth( 400 );
        choosemaster->setMinimumHeight( 300 );
    }
    if( choosemaster ) {
        choosemaster->getDataTable()->setSelectedIDs(v);
        choosemaster->showModalFor ( parent, false, true );
        if( !choosemaster->wasCancelled() && choosemaster->getDataTable()->getSelectedIDs().size() ) {
            v = choosemaster->getDataTable()->getSelectedIDs();
            ret = true;
        }
        delete choosemaster;
    } else {
        FrmBase::msgError( getPackageName(),
                           Xtring::printf( _("No se ha podido crear un formulario para la tabla de %s"),
                                           rec->getTableDefinition()->getDescPlural().c_str() ) );
    }
    return ret;
}


/**
 * @brief Busca un registro a partir de un código, una descripción o
 * un código secundario ( si el flag dbApplication::SeekCodeSecundary está activo)
 *
 * @param rec el registro en el que se busca
 * @param owner el formulario padre, si existe, para enviarlo al formulario choose o edit_Rec
 * @param fldcod el nombre del campo de código del formulario
 * @param code el valor del código
 * @param flddesc el nombre del campo de descripción que se usa en el formulario
 * @param desc el valor de la descripción
 * @param cond condiciones adicionales para la búsqueda
 * @param flags \sa dbRecord::SeekCodeFlags
 * @return dbRecordID
 */

dbRecordID dbApplication::seekCode( dbRecord *rec, QWidget *owner,
                                    const Xtring &fldcod, const Variant &code,
                                    const Xtring &flddesc, const Variant &desc,
                                    const Xtring &cond, dbRecord::SeekCodeFlags flags )
{
    int nvalues;
    sCodeNotFound = code.toString();
    sDescNotFound = desc.toString();
    sSeekCodeRecordIDs.clear();
    Xtring lastCond = Xtring::null;
    dbRecordID recid = rec->seekCode(nvalues, fldcod, code, flddesc, desc, cond, flags, lastCond);
    if ( recid == 0 || ( nvalues == 1 && (flags & dbRecord::AskIfFoundOne ) ) ) {
        Xtring addcond = rec->getFilter( "", cond );
        Xtring message, message_cond;
        if( nvalues == 0 ) {
            message = Xtring::printf( _("No se han encontrado %s que contengan \"%s\""),
                                      rec->getTableDefinition()->getDescPlural().c_str(),
                                      sDescNotFound.isEmpty() ? sCodeNotFound.c_str() : sDescNotFound.c_str() );
            if( addcond.isEmpty() )
                message_cond = _("Mostrando todos los registros");
            else
                message_cond = _("La tabla está filtrada. Pulsa CTRL+L para eliminar el filtro.\n") + addcond;
        } else {
            message = Xtring::printf( _("%s que contienen \"%s\""),
                                      rec->getTableDefinition()->getDescPlural().c_str(),
                                      sDescNotFound.isEmpty() ? sCodeNotFound.c_str() : sDescNotFound.c_str() );
        }
        if( (flags & dbRecord::DontShowBrowse) && !( nvalues == 1 && (flags & dbRecord::AskIfFoundOne ) ) ) {
            showOSD( message, Xtring::null );
        } else if( flags & dbRecord::InsertIfNotFound && nvalues == 0) {
            message_cond = Xtring::printf( _("Añadiendo %s"),
                                           DBAPP->getTableDescSingular( rec->getTableName(), "una nueva").c_str() );
            showOSD( message, message_cond );
            rec->setNew( true );
            rec->clear( true ); // set custom default values
            FrmEditRec *frmedit = createEditForm(0, rec, 0, DataTable::inserting,
                                                 dbApplication::EditFlags(dbApplication::simpleEdition), owner );
            frmedit->showModalFor( owner, false, true );
            recid = rec->getRecordID();
            delete frmedit;
        } else {
            showOSD( message, message_cond );
            dbViewDefinitionDict views;
            getDatabase()->getViewsForTable ( rec->getTableName(), views );
            dbRecordDataModel dm ( rec, views, lastCond );
            if( flags & dbRecord::SeekCodeMultiple ) {
                if( chooseMulti(sSeekCodeRecordIDs,
                                static_cast<FrmEditRecMaster *>(0), rec, &dm,
                                (flags & dbRecord::SeekCodeReadonly) ? dbApplication::readOnly : dbApplication::editNone,
                                owner ) )
                    if( sSeekCodeRecordIDs.size() ) {
                        recid = sSeekCodeRecordIDs[0];
                        rec->read(recid);
                    }
            } else
                recid = choose ( 0, rec, &dm,
                                 (flags & dbRecord::SeekCodeReadonly) ? dbApplication::readOnly : dbApplication::editNone,
                                 owner );
        }
    }
    sCodeNotFound = sDescNotFound = Xtring::null;
    return recid;
}

#if 0
dbRecordID dbApplication::seekCode ( dbRecord *rec, QWidget *owner,
                                     const Xtring &fldcod, int code,
                                     const Xtring &_flddesc, const Xtring &desc,
                                     const Xtring &cond, SeekCodeFlags flags,
                                     LineEdit *editAlt, const Xtring &fldaltname )
{
    Xtring addcond, swheres[6];
    int nwheres=0, nvalues, icond;
    dbRecordID recid=0;
    addcond = rec->getFilter( "", cond );

    sCodeNotFound = Xtring::number(code);
    sDescNotFound = desc;
    sSeekCodeRecordIDs.clear();
    Xtring flddesc;
    bool usejoinedtables = false;
    if( flddesc.find( ".") != Xtring::npos ) {
        usejoinedtables = true;
        flddesc = _flddesc;
    } else
        flddesc = rec->getTableName() + "." + _flddesc;
    if ( !flddesc.isEmpty() && !fldcod.isEmpty() )
    {
        if ( code!=0 && !desc.isEmpty() )
        {
            swheres[nwheres++] = fldcod + "=" + rec->getConnection()->toSQL ( code )
                                 + " AND " + flddesc + "=" + rec->getConnection()->toSQL ( desc );
            swheres[nwheres++] = fldcod + "=" + rec->getConnection()->toSQL ( code )
                                 + " AND " + rec->getConnection()->toSQLLike ( flddesc, desc );
            swheres[nwheres++] = rec->getConnection()->toSQLStartLike ( fldcod, Xtring::number ( code ) )
                                 + " AND " + flddesc + "=" + rec->getConnection()->toSQL ( desc );
            swheres[nwheres++] = rec->getConnection()->toSQLStartLike ( fldcod, Xtring::number ( code ) )
                                 + " AND " + rec->getConnection()->toSQLLike ( flddesc, desc );
            swheres[nwheres++] = rec->getConnection()->toSQLLike ( fldcod, Xtring::number ( code ) )
                                 + " AND " + rec->getConnection()->toSQLLike ( flddesc, desc );
        }
        else if ( code!=0 )
        {
            if( flags & FindCodeInDesc ) {
                swheres[nwheres++] = rec->getConnection()->toSQLLike ( fldcod, Xtring::number( code ) )
                                     + " OR " + rec->getConnection()->toSQLLike ( flddesc, Xtring::number( code ) );
            } else {
                swheres[nwheres++] = fldcod + "=" + rec->getConnection()->toSQL ( code );
                swheres[nwheres++] = rec->getConnection()->toSQLStartLike ( fldcod, Xtring::number ( code ) );
                swheres[nwheres++] = rec->getConnection()->toSQLLike ( fldcod, Xtring::number ( code ) );
                swheres[nwheres++] = flddesc + "=" + rec->getConnection()->toSQL ( code );
                swheres[nwheres++] = rec->getConnection()->toSQLLike ( flddesc, Xtring::number ( code ) );
            }
        }
        else
        {
            swheres[nwheres++] = flddesc + "=" + rec->getConnection()->toSQL ( desc );
            swheres[nwheres++] = rec->getConnection()->toSQLLike ( flddesc, desc );
            swheres[nwheres++] = fldcod + "=" + rec->getConnection()->toSQL ( desc );
            swheres[nwheres++] = rec->getConnection()->toSQLStartLike ( fldcod, desc );
            swheres[nwheres++] = rec->getConnection()->toSQLLike ( fldcod, desc );
        }
    }
    else if ( !flddesc.isEmpty() )
    {
        swheres[nwheres++] = flddesc + "=" + rec->getConnection()->toSQL ( desc );
        swheres[nwheres++] = rec->getConnection()->toSQLLike ( flddesc, desc );
    }
    else
    {
        swheres[nwheres++] = fldcod + "=" + rec->getConnection()->toSQL ( code );
        swheres[nwheres++] = rec->getConnection()->toSQLStartLike ( fldcod, Xtring::number ( code ) );
        swheres[nwheres++] = rec->getConnection()->toSQLLike ( fldcod, Xtring::number ( code ) );
    }
    nvalues = icond = 0;
    do
    {
        // The fields 'fldcod' & 'flddesc' are necessary for the choose form
        Variant vrecid = 0;
        Xtring select = "SELECT " + rec->getTableName() + ".ID,"
                        + rec->getTableName() + "." + fldcod;
        if ( !flddesc.isEmpty() )
            select += "," + flddesc;
        // If cond is not empty, chances are that this condition referres other tables
        select += " FROM " + ( (!cond.isEmpty() || usejoinedtables) ? rec->getJoinedFrom( false /*left*/ ) : rec->getTableName()  );
        select += " WHERE ";
        if ( !addcond.isEmpty() )
            select += addcond + "AND";
        select += "(" + swheres[icond] + ")";
        try
        {
            Variant dummycode ( Variant::tString ), dummydesc ( Variant::tString );
            nvalues = rec->getConnection()->selectValues ( select, &vrecid, &dummycode, &dummydesc );
            if ( nvalues == 1 ) {
                recid = vrecid.toInt();
                break;
            } else if( nvalues > 1 )
                break;
        }
        catch ( dbError &e )
        {
            nvalues = 0;
        }
        icond++;
    }
    while ( icond<nwheres );
    if ( recid == 0 || ( nvalues == 1 && (flags & AskIfFoundOne ) ) ) {
        Xtring message, message_cond;
        if( nvalues == 0 ) {
            message = Xtring::printf( _("No se han encontrado %s que contengan \"%s\""),
                                      rec->getTableDefinition()->getDescPlural().c_str(),
                                      sDescNotFound.isEmpty() ? sCodeNotFound.c_str() : sDescNotFound.c_str() );
            if( addcond.isEmpty() )
                message_cond = "Mostrando todos los registros";
            else
                message_cond = "La tabla está filtrada. Pulsa CTRL+L para eliminar el filtro.\n" + addcond;
        } else
            message = Xtring::printf( _("%s que contienen \"%s\""),
                                      rec->getTableDefinition()->getDescPlural().c_str(),
                                      sDescNotFound.isEmpty() ? sCodeNotFound.c_str() : sDescNotFound.c_str() );
        if( (flags & DontShowBrowse) ) {
            showOSD( message, Xtring::null );
        } else if( flags & InsertIfNotFound && nvalues == 0) {
            message_cond = Xtring::printf( _("Añadiendo %s"),
                                           DBAPP->getTableDescSingular( rec->getTableName(), "una nueva").c_str() );
            showOSD( message, message_cond );
            rec->setNew( true );
            rec->clear( true ); // set custom default values
            FrmEditRec *frmedit = createEditForm(0, rec, 0, DataTable::inserting,
                                                 dbApplication::EditFlags(dbApplication::simpleEdition), owner );
            frmedit->showModalFor( owner, false, true );
            recid = rec->getRecordID();
            delete frmedit;
        } else {
            showOSD( message, message_cond );
            dbViewDefinitionDict views;
            getDatabase()->getViewsForTable ( rec->getTableName(), views );
            if ( icond < nwheres ) {
                if( ! addcond.isEmpty() )
                    addcond += "AND ";
                addcond += "(" + swheres[icond] + ")";
            }
            dbRecordDataModel dm ( rec, views, addcond );
            if( flags & SeekCodeMultiple ) {
                if( chooseMulti(sSeekCodeRecordIDs,
                                static_cast<FrmEditRecMaster *>(0),
                                rec, &dm, dbApplication::editNone, owner ) )
                    if( sSeekCodeRecordIDs.size() )
                        recid = sSeekCodeRecordIDs[0];
            } else
                recid = choose ( 0, rec, &dm, dbApplication::editNone, owner );
        }
    } else
        rec->read ( recid );
    sCodeNotFound = sDescNotFound = Xtring();
    return recid;
}
#endif

dbModule *dbApplication::findModule ( const Xtring &name ) const
{
    dbModule *p = mModules[name];
    if ( !p )
        _GONG_DEBUG_PRINT(1, "Module " + name + " no encontrado. Los módulos son: " + mModules.toString() );
    return p;
}

bool dbApplication::addModule( dbModule *module )
{
    return mModules.insert ( module->getUnixName(), module );
}

void dbApplication::writeSettings()
{
    if( !isReadOnly() ) {
        if( pUserGlobalSettings )
            pUserGlobalSettings->write();
        if( pGlobalSettings )
            pGlobalSettings->write();
    }
    if( pUserLocalSettings )
        pUserLocalSettings->write();
    if( pMachineSettings )
        pMachineSettings->write();
}

/**
 * @brief Gets the value of a setting from its key
 *
 * The order of precedende of the settings is:
 *    1) UserLocalSettings. Stored in a file in the HOME directory of the unix user.
 *    2) UserGlobalSettings. Stored in the database per user (not used at the moment)
 *    3) GlobalSettings.- Stored in the database, without user
 *    4) MachineSettings.- Read-only, installed with the application
 *
 * @param settingname ...
 * @param defaultvalue ...
 * @return Variant
 **/
Variant dbApplication::getAppSetting(const Xtring &settingname, const Variant &defaultvalue) const
{
    Variant v( Variant::tInvalid );
    if( pUserLocalSettings )
        v = pUserLocalSettings->getValue( settingname );
    if( !v.isValid() && pUserGlobalSettings )
        v = pUserGlobalSettings->getValue( settingname );
    if( !v.isValid() && pGlobalSettings )
        v = pGlobalSettings->getValue( settingname );
    if( !v.isValid() )
        v = pMachineSettings->getValue( settingname );
    if( !v.isValid() ) {
        if( defaultvalue.isValid() || settingname.startsWith("MODULE") )
            return defaultvalue;
        else {
            // Search the default value in the application settings
            const dbModuleSetting *ms = getModuleSettings();
            while( ms && ms->type != dbModuleSetting::None ) {
                if( strcmp( ms->key, settingname.c_str() ) == 0 ) {
                    return Variant(ms->defaultvalue);
                }
                ms++;
            }
            return defaultvalue;
        }
    } else
        return v;
}

Xtring dbApplication::getAppSettingConcat(const Xtring &settingname,
        const Xtring &concat, bool fromglobaltolocal,
        const Xtring &defaultvalue) const
{
    Xtring vcomb;
    std::vector<Settings *>settings;
    if( fromglobaltolocal ) {
        if( pGlobalSettings )
            settings.push_back(pGlobalSettings);
        if( pUserGlobalSettings )
            settings.push_back(pUserGlobalSettings);
        if( pUserLocalSettings )
            settings.push_back(pUserLocalSettings);
        settings.push_back(pMachineSettings);
    } else {
        settings.push_back(pMachineSettings);
        if( pUserLocalSettings )
            settings.push_back(pUserLocalSettings);
        if( pUserGlobalSettings )
            settings.push_back(pUserGlobalSettings);
        if( pGlobalSettings )
            settings.push_back(pGlobalSettings);
    }
    for( unsigned int i = 0; i < settings.size(); ++i ) {
        if( settings[i] ) {
            Variant v = settings[i]->getValue(settingname, Variant());
            if( v.isValid() ) {
                if( !vcomb.isEmpty() )
                    vcomb += concat;
                vcomb += v.toString();
            }
        } else {
            _GONG_DEBUG_WARNING( "Trying to use a setting before creating it" );
        }
    }
    if( vcomb.isEmpty() ) {
        return defaultvalue;
    } else
        return vcomb;
}

bool dbApplication::readMachineSettings( const Xtring &filename)
{
    return pMachineSettings->read( filename );
}

bool dbApplication::readUserLocalSettings( const Xtring &filename)
{
    return pUserLocalSettings->read( filename );
}

void dbApplication::setDDDFromConfig(dbDefinition *dbdef)
{
    if( !dbdef )
        dbdef = getDatabase();
    for( unsigned int nt = 0; nt < dbdef->getTables().size(); nt++ ) {
        dbTableDefinition *tbldef = dbdef->getTableDefinition(nt);
        Variant v = getAppSetting( "DBDEF.TABLE." + tbldef->getName() + ".DESC_SINGULAR" );
        if( v.isValid() )
            tbldef->setDescSingular( v.toString() );
        v = getAppSetting( "DBDEF.TABLE." + tbldef->getName() + ".DESC_PLURAL" );
        if( v.isValid() )
            tbldef->setDescPlural( v.toString() );
        v = getAppSetting( "DBDEF.TABLE." + tbldef->getName() + ".FEMENINA" );
        if( v.isValid() )
            tbldef->setFemenina( v.toBool() );
        for( unsigned int nf = 0; nf < tbldef->getFieldDefinitions().size(); nf++ ) {
            dbFieldDefinition *flddef = tbldef->getFieldDefinition(nf);
            v = getAppSetting( "DBDEF.FIELD." + tbldef->getName() + "." + flddef->getName() + ".CAPTION" );
            if( v.isValid() && !v.toString().isEmpty() )
                flddef->setCaption( v.toString() );
            v = getAppSetting( "DBDEF.FIELD." + tbldef->getName() + "." + flddef->getName() + ".DESCRIPTION" );
            if( v.isValid() && !v.toString().isEmpty() )
                flddef->setDescription( v.toString() );
            v = getAppSetting( "DBDEF.FIELD." + tbldef->getName() + "." + flddef->getName() + ".WIDTH" );
            if( v.isValid() && v.toInt() != 0)
                flddef->setDisplayWidth( v.toInt() );
            v = getAppSetting( "DBDEF.FIELD." + tbldef->getName() + "." + flddef->getName() + ".DEFAULTVALUE" );
            if( v.isValid() && !v.toString().isEmpty() )
                flddef->setDefaultValue( v.toString() );
            v = getAppSetting( "DBDEF.FIELD." + tbldef->getName() + "." + flddef->getName() + ".STYLE" );
            if( v.isValid() && !v.toString().isEmpty() )
                flddef->setStyle( v.toString() );
            // If a field can not be null, it can not be forced to be null
            v = getAppSetting( "DBDEF.FIELD." + tbldef->getName() + "." + flddef->getName() + ".CANBENULL" );
            if( v.isValid() && !v.toString().isEmpty() ) {
                if( flddef->canBeNull() == false && v.toBool() == true ) {
                    _GONG_DEBUG_WARNING( "The field " + flddef->getFullName() + " can not be forced to be null" );
                } else {
                    flddef->setCanBeNull( v.toBool() );
                }
            }
            v = getAppSetting( "DBDEF.FIELD." + tbldef->getName() + "." + flddef->getName() + ".VISIBLE" );
            if( v.isValid() && !v.toString().isEmpty() )
                flddef->setVisible( v.toBool() );
            v = getAppSetting( "DBDEF.FIELD." + tbldef->getName() + "." + flddef->getName() + ".READONLY" );
            if( v.isValid() && !v.toString().isEmpty() )
                flddef->setReadOnly( v.toBool() );
        }
    }
}

void dbApplication::setStylesFromConfig(dbDefinition* pdb)
{
    if( !pdb )
        pdb = getDatabase();
	SettingsDict styles;
    getSettingsValues("DBDEF.STYLE", styles);
    for( SettingsDict::const_iterator stit = styles.begin();
            stit != styles.end(); ++stit ) {
        Xtring stylename = stit->first.mid( stit->first.find_last_of('.') + 1 );
        pdb->addStyleFromString( stylename, stit->second.toString() );
    }
}

Variant dbApplication::getMachineSetting(const Xtring& settingname, const Variant& defaultvalue) const
{
    return pMachineSettings->getValue( settingname, defaultvalue );
}

Variant dbApplication::getUserLocalSetting(const Xtring& settingname, const Variant& defaultvalue) const
{
    return pUserLocalSettings->getValue( settingname, defaultvalue );
}

Variant dbApplication::getUserGlobalSetting(const Xtring& settingname, const Variant& defaultvalue) const
{
    return pUserGlobalSettings->getValue( settingname, defaultvalue );
}

Variant dbApplication::getGlobalSetting(const Xtring& settingname, const Variant& defaultvalue) const
{
    return pGlobalSettings->getValue( settingname, defaultvalue );
}

int dbApplication::getSettingsValues(const Xtring &key_pattern, SettingsDict &result)
{
    Xtring key_pattern_upper = key_pattern.upper();
    std::vector<Settings *>settings;
    if( pUserGlobalSettings )
        settings.push_back(pUserGlobalSettings);
    if( pGlobalSettings )
        settings.push_back(pGlobalSettings);
    settings.push_back(pUserLocalSettings);
    settings.push_back(pMachineSettings);
    for( std::vector<Settings *>::const_iterator set_it = settings.begin();
            set_it != settings.end(); ++ set_it ) {
        for( SettingsDict::const_iterator val_it = (*set_it)->allSettings().begin();
                val_it != (*set_it)->allSettings().end(); ++ val_it ) {
            Xtring key = (*val_it).first;
            Xtring key_upper = key.upper();
            _GONG_DEBUG_PRINT(6, "Buscando patrón '" + key_pattern_upper
                              + "' en la entrada de setting '" + key_upper + "'" );
            if( key_upper == key_pattern_upper
                    || key_upper.startsWith( key_pattern_upper + "." )
                    || key_upper.endsWith( "." + key_pattern_upper )
                    || key_upper.find( "." + key_pattern_upper + ".") != Xtring::npos ) {
                _GONG_DEBUG_PRINT(5, "Encontrado patrón '" + key_pattern_upper
                                  + "' en la entrada de setting '" + key_upper + "' con valor '" + val_it->second.toString() );
                if( result.find( key ) == result.end() )
                    result.insert( key, val_it->second ) ;
            }
        }
    }
    return result.size();
}

bool dbApplication::updateSetting(const Xtring &settingname, const Variant &val)
{
    _GONG_DEBUG_PRINT(5, "Updating setting " + settingname + " = " + val.toString() );
    std::vector<Settings *>settings;
    if( pUserGlobalSettings )
        settings.push_back(pUserGlobalSettings);
    if( pGlobalSettings )
        settings.push_back(pGlobalSettings);
    settings.push_back(pUserLocalSettings);
    for( std::vector<Settings *>::const_iterator set_it = settings.begin();
            set_it != settings.end(); ++ set_it ) {
        Settings *st = *set_it;
        _GONG_DEBUG_PRINT(6, "in settings " + st->getName() );
        if( st->getValue( settingname ).isValid() ) {
            st->setValue( settingname, val );
            return true;
        } else {
#ifdef _GONG_DEBUG
            std::cout << st->allSettings().toString() << std::endl;
#endif
        }
    }
    pUserLocalSettings->setValue( settingname, val );
    return false;
}

bool dbApplication::removeSetting(const Xtring &settingname, bool global)
{
    _GONG_DEBUG_PRINT(5, "Removing setting " + settingname );
    std::vector<Settings *>settings;
    if( global ) {
        if( pUserGlobalSettings )
            settings.push_back(pUserGlobalSettings);
        if( pGlobalSettings )
            settings.push_back(pGlobalSettings);
    }
    settings.push_back(pUserLocalSettings);
    for( std::vector<Settings *>::const_iterator set_it = settings.begin();
            set_it != settings.end(); ++ set_it ) {
        Settings *st = *set_it;
        _GONG_DEBUG_PRINT(6, "in settings " + st->getName() );
        st->removeSetting( settingname );
    }
    return false;
}


bool dbApplication::concatMachineSetting(const Xtring &settingname,
        const Xtring &val, const Xtring &sep)
{
    bool ret = true;
    Xtring oldvalue = pMachineSettings->getValue( settingname ).toString();
    if( !oldvalue.isEmpty() ) {
        XtringList values;
        oldvalue.tokenize(values, sep);
        if( values.contains( val ) )
            return false;
        oldvalue = sep + oldvalue;
        ret = false;
    }
    pMachineSettings->setValue( settingname, Variant(val + oldvalue) );
    return ret;
}

bool dbApplication::concatUserLocalSetting(const Xtring &settingname,
        const Xtring &val, const Xtring &sep)
{
    bool ret = true;
    Xtring oldvalue = pUserLocalSettings->getValue( settingname ).toString();
    if( !oldvalue.isEmpty() ) {
        XtringList values;
        oldvalue.tokenize(values, sep);
        if( values.contains( val ) )
            return false;
        oldvalue = sep + oldvalue;
        ret = false;
    }
    pUserLocalSettings->setValue( settingname, Variant(val + oldvalue) );
    return ret;
}

/**
 * @brief Looks for view definitions in the config values and add them to the database
 *
 * View keys are in the form: DBDEF.VIEW.TABLENAME.CAPTION, although the TABLENAME can be omitted
 * as the one that counts is the first one in the FROM clause
 *
 * @param dbdef ...
 * @return int
 **/
int dbApplication::setViewsFromConfig(dbDefinition *dbdef)
{
    if( !dbdef )
        dbdef = getDatabase();
    SettingsDict views;
    dbdef->getViews().clear();
    getSettingsValues("VIEW", views);
    for( SettingsDict::const_iterator viewit = views.begin();
            viewit!=views.end(); ++viewit ) {
        Xtring viewname = viewit->first.mid( viewit->first.find_last_of('.') + 1 );
        dbdef->addViewFromString( viewname, "SELECT " + viewit->second.toString(),
                                  "CONFIG:" + viewit->first );
    }
    return views.size();
}

/**
 * @brief Does not change mDatabaseChanged
 *
 * @param programdb ...
 * @param sqldbname ...
 * @param purging ...
 * @return Xtring
 **/
Xtring dbApplication::upgradeDatabase( dbConnection *conn, dbDefinition* programdb,
                                       const gong::Xtring& sqldbname, bool purging, bool silent)
{
    waitCursor( true );
    dbDefinition *oldschema = dbDefinition::fromSQLSchema( conn, sqldbname);
    Xtring diff = programdb->sameSQLSchema( oldschema, conn, purging );
    try {
        if( !diff.isEmpty() ) {
            _GONG_DEBUG_PRINT(2, diff );
            XtringList querys;
            diff.tokenize( querys, ";\n" );
            bool doit = true;
            if( !silent )
                doit = FrmBase::msgYesNoLarge( _("Actualización de la base de datos"),
                                               Xtring::printf( _("La base de datos de %1s ('%2s') no está actualizada.\n¿Quieres actualizarla ahora?"),
                                                       getPackageName(), sqldbname.c_str() ),
                                               diff );
            if( doit ) {
                DBAPP->showStickyOSD( DBAPP->getPackageString(), _("Actualizando la base de datos"), 0 );
                purging = false; // Do not create the indexes again later
                programdb->dropIndexes(conn, false, true);
                conn->exec( querys );
                // Las líneas que comienzan con # son comentarios
                if( diff.find("# CREATE TABLE " ) != Xtring::npos ) {
                    // Creates new tables, the existing ones are not touched
                    programdb->createTables( conn, Xtring(), true );
                } else {
                    programdb->createIndexes(conn, true);
                }
            }
        }
        if( purging ) {
            DBAPP->showStickyOSD( DBAPP->getPackageString(), _("Regenerando los índices"), 0 );
            programdb->dropIndexes(conn, false, true);
            programdb->createIndexes(conn, true);
            DBAPP->showStickyOSD( DBAPP->getPackageString(), _("Índices regenerados") );
        } else {
			DBAPP->hideOSD();
		}
    } catch( dbError e ) {
        FrmBase::msgError( getPackageString(), e.what() );
    }
    delete oldschema;
    resetCursor();
    return diff;
}

void dbApplication::setReadOnly(bool ro)
{
    mReadOnly = ro;
    if( getMainWindow() ) {
        if( mReadOnly && !mTitle.endsWith( _(" [ Bloqueada ]") ) ) {
            mTitle += _(" [ Bloqueada ]");
        } else if( !mReadOnly && mTitle.endsWith( _(" [ Bloqueada ]") ) ) {
            mTitle = mTitle.mid(0, mTitle.length() - strlen( _(" [ Bloqueada ]") ) );
        }
        getMainWindow()->setWindowTitle( toGUI( mTitle ) );
    }
}

bool dbApplication::preventOperationIf(bool cond, const Xtring& operation, const Xtring &reason)
{
    if( cond ) {
        FrmBase::msgError( getMainWindow() ? getMainWindow() : 0,
                           Xtring::printf( _("No se puede ejecutar la operación '%s'\nporque '%s'"),
                                           operation.c_str(), reason.c_str() ) );
    }
    return cond;
}


dbRecordPermissions dbApplication::getRecordPermissions(dbRecord* rec, dbUser* user)
{
    // TODO: Call plugins to get the permissions
    dbRecordPermissions r;
    r.setAll(true);
    return r;
}


const Xtring dbApplication::getTableDescSingular(const Xtring &tablename, const Xtring &demostrativo)
{
    /// TODO: getSettings->getValue("LANG.DEMOSTRATIVOS")
    /// TODO: if demostrativo is lowercase, put desc in lowercase also
    static const char *demostrativos[][2] = {
        { _("la"), _("el") },
        { _("La"), _("El") },
        { _("una"), _("un") },
        { _("Una"), _("Un") },
        { _("esta"), _("este") },
        { _("Esta"), _("Este") },
        { _("esa"), _("ese") },
        { _("Esa"), _("Ese") },
        { _("otra"), _("otro") },
        { _("Otra"), _("Otro") },
        { _("de la"), _("del") },
        { _("toda la"), _("todo el") },
        { _("Toda la"), _("Todo el") },
        { _("una nueva"), _("un nuevo") }
    };
    dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablename );
    if( tbldef != 0 ) {
        if( demostrativo.isEmpty() )
            return tbldef->getDescSingular();
        else if( demostrativo == "<" )
            return tbldef->getDescSingular().lower();
        for ( unsigned int i=0; i<sizeof(demostrativos)/sizeof(demostrativos[0]); i++ ) {
            if ( demostrativo == demostrativos[i][0] ) {
                if ( tbldef->isFemenina() )
                    return Xtring(demostrativos[i][0]) + " " + tbldef->getDescSingular().lower();
                else
                    return Xtring(demostrativos[i][1]) + " " + tbldef->getDescSingular().lower();
            }
        }
    }
    if( !demostrativo.isEmpty() )
        return demostrativo + " " + tablename;
    else
        return tablename;
}

const Xtring dbApplication::getTableDescPlural(const Xtring &tablename, const Xtring &demostrativo)
{
    static const char *demostrativos[][2] = {
        { _("las"), _("los") },
        { _("Las"), _("Los") },
        { _("unas"), _("unos") },
        { _("Unas"), _("Unos") },
        { _("estas"), _("estos") },
        { _("Estas"), _("Estos") },
        { _("otras"), _("otros") },
        { _("Otras"), _("Otros") },
        { _("esas"), _("esos") },
        { _("Esas"), _("Esos") },
        { _("todas las"), _("todos los") },
        { _("Todas las"), _("Todos los") }
    };
    dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( tablename );
    if( tbldef != 0 ) {
        if( demostrativo.isEmpty() )
            return tbldef->getDescPlural();
        else if( demostrativo == "<" )
            return tbldef->getDescPlural().lower();
        for ( unsigned int i=0; i<sizeof(demostrativos)/sizeof(demostrativos[0]); i++ ) {
            if ( demostrativo == demostrativos[i][0] ) {
                if ( tbldef->isFemenina() )
                    return Xtring(demostrativos[i][0]) + " " + tbldef->getDescPlural().lower();
                else
                    return Xtring(demostrativos[i][1]) + " " + tbldef->getDescPlural().lower();
            }
        }
    }
    if( !demostrativo.isEmpty() )
        return demostrativo + " " + tablename;
    else
        return tablename;
}

long int dbApplication::version2Long(const Xtring& version)
{
    XtringList versions;
    version.tokenize( versions, "." );
	while( versions.size() < 5 ) {
		versions.push_back("0");
	}
    long nversion = 0;
    for ( unsigned int nitem = 0; nitem < versions.size(); nitem ++ )
        nversion = nversion * 100 + versions[nitem].toInt();
    return nversion;
}


} // namespace gong
