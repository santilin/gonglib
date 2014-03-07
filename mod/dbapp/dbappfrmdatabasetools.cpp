#include <errno.h>
#include <gonggettext.h>
#include <gongfileutils.h>
#include <gongpushbutton.h>
#include "dbappmainwindow.h"
#include "dbapprecmetadbdata.h"
#include "dbappdbapplication.h"
#include "dbappfrmdatabasetools.h"
#include "gongdbfielddefinition.h"

namespace gong {

class FrmBackupForm: public FrmCustom
{
public:
    enum BackupModes { backup, restore, lock, unlock };
    FrmBackupForm( BackupModes mode, QWidget *parent, const Xtring &title )
        : FrmCustom( parent )
    {
        setTitle( title );
        if( mode == backup || mode == lock ) {
            pEditBackupPath = addDirNameBox( parent, _("Carpeta de las copias") );
            pEditBackupPath->setFileName( DBAPP->getAppSetting( "SYSTEM.DBBACKUP.PATH" ).toString() );
        }
        if( mode == restore ) {
            pEditBackupPath = addFileNameBox( parent, _("Carpeta de las copias") );
            pEditBackupPath->setFileName( DBAPP->getAppSetting( "SYSTEM.DBBACKUP.PATH" ).toString() );
        }
        if( mode == backup || mode == restore || mode == lock ) {
            pEditPassword = addInput( parent, _("Contraseña de la base de datos"), Xtring(), "PASSWORD" );
            setPassword( DBAPP->getDbUserPassword() );
        }
        if( mode == backup ) {
            pEditInterval = addInput( parent, _("Intervalo en días para las copias automáticas"),
                                      DBAPP->getAppSetting( "SYSTEM.DBBACKUP.INTERVAL", 0 ).toInt() );
            pOnlyGongTables = addCheckBox( parent, Xtring::printf( _("Sólo las tablas de %s"), DBAPP->getPackageString().c_str() ), true );
            pAutomatic = addCheckBox( parent, _("Realizar las copias automáticamente"),
                                      DBAPP->getAppSetting( "SYSTEM.DBBACKUP.AUTOMATIC", 0 ).toBool() );
        }
        if( mode == lock || mode == unlock ) {
            pEditLockDesc = addInput( parent, "Descripción del bloqueo", Xtring() );
            pEditLockUser = addInput( parent, "Usuaria que bloquea", DBAPP->getDbUser() );
            pEditLockPassword = addInput( parent,
                                          mode==lock ? _("Contraseña para el bloqueo") : _("Contraseña para el desbloqueo"),
                                          Xtring(), "PASSWORD" );
            if( mode == unlock ) {
                pEditLockDesc->setReadOnly( true );
                pEditLockUser->setReadOnly( true );
                pFocusWidget = pEditLockPassword;
            }
        }
    }
    Xtring getPassword() const {
        return pEditPassword->toString();
    }
    void setPassword( const Xtring &password ) {
        pEditPassword->setText( password );
    }
    Xtring getBackupPath() const {
        return pEditBackupPath->getFileName();
    }
    int getInterval() const {
        return pEditInterval->toInt();
    }
    bool isOnlyGongTables() const {
        return pOnlyGongTables->isOn();
    }
    Xtring getLockPassword() const {
        return pEditLockPassword->toString();
    }
    Xtring getLockUser() const {
        return pEditLockUser->toString();
    }
    Xtring getLockDesc() const {
        return pEditLockDesc->toString();
    }
    void setLockDesc( const Xtring &desc ) {
        pEditLockDesc->setText( desc );
    }
    bool isAutomatic() const {
        return pAutomatic->isOn();
    }

private:
    FileNameBox *pEditBackupPath;
    LineEdit *pEditPassword, *pEditInterval;
    LineEdit *pEditLockUser, *pEditLockDesc, *pEditLockPassword;
    CheckBox *pOnlyGongTables, *pAutomatic;
};

FrmDatabaseTools::FrmDatabaseTools(QWidget* parent, const char* name, WidgetFlags fl)
    : FrmCustom(parent, name, fl)
{
    if( strempty(name) )
        setObjectName("FrmDatabaseTools");
    setTitle( "Herramientas de la base de datos" );
    pButtonBackup = addButton( this, _("Copia de seguridad") );
    pButtonRestore = addButton( this, _("Recuperar copia") );
    pButtonLockDatabase = addButton( this, _("Bloquear la base de datos") );
    pButtonUnlockDatabase = addButton( this, _("Desbloquear la base de datos") );
    pButtonUpgrade = addButton( this, _("Limpiar") );
    pButtonCheck = addButton( this, _("Regenerar índices") );
    pButtonCheckDDD = addButton( this, _("Comprobar diccionario de datos") );
    pushCancel->setText( _("Cerrar") );
    pushAccept->setVisible( false );
}

bool FrmDatabaseTools::dump( const Xtring &dumpfname, const Xtring &database,
                             const Xtring &user, const Xtring &host,
                             const Xtring &password, const XtringList &tables,
                             Xtring &errors )
{
    DBAPP->waitCursor( true );
    Xtring dumpcommand = DBAPP->getAppSetting( "SYSTEM.DBBACKUP.COMMAND").toString();
    if( dumpcommand.isEmpty() )
        dumpcommand = "mysqldump -u $user -p\"$password\" -h \"$host\" $database $tables > $destfile";
    dumpcommand.replace( "$user", user );
    if( !host.isEmpty() )
        dumpcommand.replace( "$host", host );
    else
        dumpcommand.replace( "$host", "localhost" );
    dumpcommand.replace( "$password", password );
    dumpcommand.replace( "$database", database );
    if( tables.size() )
        dumpcommand.replace( "$tables", tables.join(" ") );
    else
        dumpcommand.replace( "$tables", "" );
    dumpcommand.replace( "$destfile", dumpfname );
    if( FileUtils::execProcess ( dumpcommand.c_str(), Xtring::null, errors ) == -1 )
        errors = _("No se ha podido ejecutar el comando:\n" + dumpcommand + "\n" + errors );
    DBAPP->resetCursor();
    return errors.trim().isEmpty();
}

bool FrmDatabaseTools::load( const Xtring &dumpfname, const Xtring &database,
                             const Xtring &user, const Xtring &host,
                             const Xtring &password, Xtring &errors )
{
    DBAPP->waitCursor( true );
    Xtring loadcommand = DBAPP->getAppSetting( "SYSTEM.DBBACKUP.RESTORE_COMMAND").toString();
    if( loadcommand.isEmpty() )
        loadcommand = "mysql -u $user -p\"$password\" -h \"$host\" $database < $origfile";
    Xtring origcommand = loadcommand;
    loadcommand.replace( "$user", user );
    if( !host.isEmpty() )
        loadcommand.replace( "$host", host );
    else
        loadcommand.replace( "$host", "localhost" );
    loadcommand.replace( "$password", password );
    loadcommand.replace( "$database", database );
    loadcommand.replace( "$origfile", dumpfname );
    if( FileUtils::execProcess ( loadcommand.c_str(), Xtring::null, errors ) == -1 )
        errors = _("No se ha podido ejecutar el comando:\n") + loadcommand + "\n" + errors ;
    DBAPP->resetCursor();
    return errors.trim().isEmpty();
}

Xtring FrmDatabaseTools::createDirAndSelectFile( const Xtring &_dir, bool automatic )
{
    Xtring dir = _dir;
    if( !dir.endsWith( "/" ) )
        dir += "/";
    if( !dir.isEmpty() ) {
        if( !FileUtils::exists( dir.c_str() ) ) {
            if( FileUtils::makePath( dir ) != 0 ) {
                FrmBase::msgError(this,
                                  Xtring::printf( _("No se ha podido crear el directorio %s"),
                                                  dir.c_str() ) );
                return Xtring::null;
            }
        }
    }
    Xtring proposedname = dir + DBAPP->getDatabase()->getName() + DateTime::currentDateTime().toString("%Y%m%d_%H%M") + ".sql";
    if( automatic )
        return proposedname;
    Xtring fname = DBAPP->getSaveFileName( _("Elija un fichero de destino"), proposedname,
                                           _("Copias de seguridad (*.sql);;Todos los archivos (*)" ),
                                           this );
    if ( !fname.isEmpty() ) {
        if ( FileUtils::extension ( fname ).isEmpty() )
            fname += ".sql";
    }
    return fname;
}

void FrmDatabaseTools::backupDatabase(bool automatic)
{
    FrmBackupForm *frmbackup = new FrmBackupForm( FrmBackupForm::backup, this,
            _("Realizar copia de seguridad") );
    if( !automatic )
        frmbackup->showModalFor( this, true, true );
    else {
        frmbackup->show();
        frmbackup->accept();
    }
    Xtring password = frmbackup->getPassword();
    int interval = frmbackup->getInterval();
    bool cancelled = frmbackup->wasCancelled();
    bool onlygongtables = frmbackup->isOnlyGongTables();
    Xtring backuppath = frmbackup->getBackupPath();
    automatic = frmbackup->isAutomatic();
    delete frmbackup;
    if( !cancelled ) {
        Xtring fname = createDirAndSelectFile( backuppath, automatic );
        if( !fname.isEmpty() ) {
            DBAPP->setUserLocalSetting( "SYSTEM.DBBACKUP.INTERVAL", interval );
            DBAPP->setUserLocalSetting( "SYSTEM.DBBACKUP.PATH", FileUtils::path( fname ) );
            DBAPP->setUserLocalSetting( "SYSTEM.DBBACKUP.AUTOMATIC", automatic );
            Xtring errors;
            XtringList alltables, validtables;
            if( onlygongtables ) {
                alltables << "METADBDATA " << DBAPP->getMasterTables() << DBAPP->getDetailTables();
 				for( XtringList::const_iterator it = alltables.begin(); it != alltables.end(); ++ it ) {
 					dbTableDefinition *tbldef = DBAPP->getDatabase()->findTableDefinition( *it );
					if( tbldef && !tbldef->isTemporary() )
						validtables << *it;
				}
			}
            if( !dump( fname, DBAPP->getDatabase()->getName(), DBAPP->getDbUser(),
                       DBAPP->getDbHost(), password, validtables, errors ) ) {
                FrmBase::msgError( this, errors );
            } else {
                Xtring message = Xtring::printf(_("Se ha generado la copia de seguridad en el fichero\n%s"), fname.c_str() );
                if( automatic )
                    DBAPP->showStickyOSD( DBAPP->getPackageString(), message );
                else
                    FrmBase::msgOk( this, message );
                DBAPP->setUserLocalSetting( "SYSTEM.DBBACKUP.LAST_BACKUP", Date::currentDate().toString("%Y-%m-%d") );
            }
        }
    }
}

void FrmDatabaseTools::lockDatabase()
{
    FrmBackupForm *frmlock = new FrmBackupForm( FrmBackupForm::lock, this,
            _("Bloquear la base de datos") );
    frmlock->showModalFor( this, true, true );
    Xtring password = frmlock->getPassword();
    bool cancelled = frmlock->wasCancelled();
    Xtring lockuser = frmlock->getLockUser();
    Xtring lockpassword = frmlock->getLockPassword();
    Xtring lockdesc = frmlock->getLockDesc();
    Xtring backuppath = frmlock->getBackupPath();
    delete frmlock;
    if( !cancelled ) {
        if( lockuser.isEmpty() || lockdesc.isEmpty() ) {
            msgError(this, _("Introduce una usuaria y una descripción para el bloqueo de la base de datos.") );
            return;
        }
        if( msgYesNo(this,
                     _("La base de datos se bloqueará ahora y sólamente se podrá desbloquear con la contraseña introducida.\n¿Quieres continuar?") ) ) {
            if( !DBAPP->getRecMetaDBData()->lock(lockuser, lockpassword, lockdesc, Date::currentDate() ) ) {
                msgError( this, _("No se ha podido bloquear la base de datos.") );
                return;
            }
        }
        Xtring fname = createDirAndSelectFile( backuppath, false );
        if( !fname.isEmpty() ) {
            Xtring errors;
            XtringList tables;
            tables << "METADBDATA " << DBAPP->getMasterTables() << DBAPP->getDetailTables();
            if( !dump( fname, DBAPP->getDatabase()->getName(), DBAPP->getDbUser(),
                       DBAPP->getDbHost(), password, tables, errors ) ) {
                FrmBase::msgError( this, errors );
            } else {
                FrmBase::msgOk( this,
                                Xtring::printf(_("Se ha generado la copia de seguridad en el fichero\n%s\n\n"
                                                 "La base de datos se ha bloqueado por lo tanto no se podrán hacer modificaciones"), fname.c_str()) );
                DBAPP->setReadOnly( true );
            }
        }
    }
}

void FrmDatabaseTools::restoreDatabase()
{
    FrmBackupForm *frmrestore = new FrmBackupForm( FrmBackupForm::restore, this,
            _("Restaurar copia de seguridad" ) );
    frmrestore->setPassword( DBAPP->getDbUserPassword() );
    frmrestore->showModalFor( this, true, true );
    Xtring password = frmrestore->getPassword();
    bool cancelled = frmrestore->wasCancelled();
    Xtring backuppath = frmrestore->getBackupPath();
    delete frmrestore;
    if( !cancelled ) {
        Xtring fname = DBAPP->getOpenFileName( _("Elige el fichero a restaurar"),
                                               backuppath,
                                               _("Copias de seguridad (*.sql);;Todos los archivos (*)" ),
                                               this );
        if ( !fname.isEmpty() ) {
            Xtring host;
            if( !DBAPP->getDbHost().isEmpty() )
                host = DBAPP->getDbHost();
            else
                host = "localhost";
            if( FrmBase::msgYesNo( this, Xtring::printf(
                                       _("Estás segura de recuperar una copia de seguridad en la base de datos '%s' localizada en el servidor '%s'?"),
                                       DBAPP->getDatabase()->getName().c_str(), host.c_str() ) ) ) {
                Xtring errors;
                if( !load( fname, DBAPP->getDatabase()->getName(), DBAPP->getDbUser(),
                           host, password, errors ) ) {
                    FrmBase::msgError ( this, errors );
                } else {
                    FrmBase::msgOk( this,
                                    Xtring::printf(_("Se ha restaurado la copia de seguridad desde el fichero\n%s.\nDebes reiniciar el programa ahora."), fname.c_str()) );
                    DBAPP->quit();
                }
            }
        }
    }
}

void FrmDatabaseTools::unlockDatabase()
{
    if( !DBAPP->getRecMetaDBData()->getValue( "LOCKED" ).toBool() ) {
        msgWarning( this, _("La base de datos no está bloqueada.") );
        return;
    }
    FrmBackupForm *frmlock = new FrmBackupForm( FrmBackupForm::unlock, this,
            _("Desbloquear la base de datos") );
    frmlock->showModalFor( this, true, true );
    Xtring lockpassword = frmlock->getLockPassword();
    frmlock->setLockDesc( DBAPP->getRecMetaDBData()->getValue("LOCKED_DESC").toString() );
    bool cancelled = frmlock->wasCancelled();
    delete frmlock;
    if( !cancelled ) {
        if( DBAPP->getRecMetaDBData()->unlock( lockpassword ) ) {
            DBAPP->setReadOnly( false );
            msgOk( this, _("La base de datos ha sido desbloqueada") );
        } else {
            msgError( this, _("La base de datos no se ha podido desbloquear.") );
        }
    }
}


void FrmDatabaseTools::upgradeDatabase()
{
    DBAPP->waitCursor( true );
    Xtring diff = DBAPP->upgradeDatabase( DBAPP->getConnection(), DBAPP->getDatabase(),
										  DBAPP->getDatabase()->getName(), true /*purging*/ );
    if( diff.isEmpty() )
        FrmBase::msgWarning(this, _( "Se ha actualizado la base de datos" ) );
    DBAPP->resetCursor();
}

void FrmDatabaseTools::checkDatabase()
{
    DBAPP->waitCursor( true );
    try {
        DBAPP->showOSD( getTitle(), _("Regenerando índices") );
        DBAPP->getDatabase()->dropIndexes( DBAPP->getConnection(), true, true );
        DBAPP->getDatabase()->createIndexes( DBAPP->getConnection(), false );
        DBAPP->resetCursor();
        DBAPP->hideOSD();
        FrmBase::msgOk( this, _("Los índices se han regenerado correctamente.") );
    } catch( dbError &e ) {
        DBAPP->resetCursor();
        DBAPP->hideOSD();
        FrmBase::msgError(this, "Error regenerando los índices.", e.getWholeError() );
    }
}

Xtring FrmDatabaseTools::checkDDDKey(const Xtring& key, const Xtring& missingvalue)
{
    Xtring ret;
    bool notfound = false;
    if( DBAPP->getAppSetting( key ).type() == Variant::tInvalid )
        notfound = true;
    else if( DBAPP->getAppSetting( key, "" ).type() == Variant::tInvalid )
        notfound = true;
    if( notfound )
        ret = key + "= \"" + missingvalue + "\";\n";
    return ret;
}

void FrmDatabaseTools::checkDDD()
{
    // Check fields that are not in the ddd
    DBAPP->waitCursor( true );
    Xtring ret, key;
    for( unsigned int nt = 0; nt < DBAPP->getDatabase()->getTables().size(); nt++ ) {
        const dbTableDefinition *tbldef = DBAPP->getDatabase()->getTableDefinition(nt);
        if( tbldef->getName() == "METADBDATA" || tbldef->getName() == "GLOBALCONFIG" )
            continue;
        key = "DBDEF.TABLE." + tbldef->getName() + ".DESC_SINGULAR";
        ret += checkDDDKey( key );
        key = "DBDEF.TABLE." + tbldef->getName() + ".DESC_PLURAL";
        ret += checkDDDKey( key );
        key = "DBDEF.TABLE." + tbldef->getName() + ".FEMENINA";
        ret += checkDDDKey( key );
        Xtring subkey;
        for( unsigned int nf = 0; nf < tbldef->getFieldCount(); nf ++ ) {
            const dbFieldDefinition *flddef = tbldef->getFieldDefinition(nf);
            if( flddef->getName().startsWith( "REC_" ) )
                continue; // TODO: sacar los nombres del timestamp behaviour
            subkey = "DBDEF.FIELD." + flddef->getFullName() + ".";
            key = subkey + "CAPTION";
            ret += checkDDDKey( key, flddef->getCaption() );
            key = subkey + "DESCRIPTION";
            ret += checkDDDKey( key, flddef->getDescription() );
            key = subkey + "WIDTH";
            ret += checkDDDKey( key, Xtring::number(flddef->getDisplayWidth()) );
            key = subkey + "DEFAULTVALUE";
            ret += checkDDDKey( key, flddef->getDefaultValue() );
            key = subkey + "STYLE";
            ret += checkDDDKey( key, flddef->getStyle() );
            key = subkey + "CANBENULL";
            ret += checkDDDKey( key, flddef->canBeNull() ? "true" : "false" );
            key = subkey + "VISIBLE";
            ret += checkDDDKey( key, flddef->isVisible() ? "true" : "false" );
            key = subkey + "READONLY";
            ret += checkDDDKey( key, flddef->isReadOnly() ? "true" : "false" );
        }
    }
    DBAPP->resetCursor();
    if( !ret.isEmpty() ) {
        FrmBase::msgOkLarge( this, _("Estos campos de la base de datos no están definidos en el diccionario de datos"),
                             ret );
    }

    DBAPP->waitCursor( true );
    ret.clear();
    SettingsDict dddkeys;
    DBAPP->getSettingsValues("DBDEF.TABLE", dddkeys);
    for( unsigned int i=0; i<dddkeys.size(); ++i ) {
        XtringList parts;
        dddkeys.getKey(i).tokenize(parts, ".");
        if( parts.size() >= 2 ) {
            Xtring tablename = parts[2];
            if( !DBAPP->getDatabase()->findTableDefinition( tablename ) )
                ret += dddkeys.getKey(i) + "\n";
        } else {
            ret += Xtring::printf( _("La clave '%s' es errónea"), dddkeys.getKey(i).c_str() );
        }
    }
    DBAPP->resetCursor();
    if( !ret.isEmpty() ) {
        FrmBase::msgOkLarge( this, _("Estas claves del diccionario de datos se refieren a tablas que no están en la base de datos"), ret );
    }

    DBAPP->waitCursor( true );
    ret.clear();
    dddkeys.clear();
    DBAPP->getSettingsValues("DBDEF.FIELD", dddkeys);
    for( unsigned int i=0; i<dddkeys.size(); ++i ) {
        XtringList parts;
        dddkeys.getKey(i).tokenize(parts, ".");
        if( parts.size() >= 3 ) {
            Xtring fullfldname = parts[2] + "." + parts[3];
            if( !DBAPP->getDatabase()->findFieldDefinition( fullfldname ) )
                ret += dddkeys.getKey(i) + "\n";
        } else {
            ret += Xtring::printf( _("La clave '%s' es errónea"), dddkeys.getKey(i).c_str() );
        }
    }
    DBAPP->resetCursor();
    if( !ret.isEmpty() ) {
        FrmBase::msgOkLarge( this, _("Estas claves del diccionario de datos se refieren a campos que no están en la base de datos"), ret );
    }
}

void FrmDatabaseTools::validate_input(QWidget* sender, bool* isvalid)
{
    FrmCustom::validate_input( sender, isvalid );
    if( sender == pButtonBackup )
        backupDatabase( false /*not automatic*/);
    else if( sender == pButtonCheck )
        checkDatabase();
    else if( sender == pButtonUpgrade )
        upgradeDatabase();
    else if( sender == pButtonRestore )
        restoreDatabase();
    else if( sender == pButtonCheckDDD )
        checkDDD();
    else if( sender == pButtonLockDatabase )
        lockDatabase();
    else if( sender == pButtonUnlockDatabase )
        unlockDatabase();
}


} // namespace gong



