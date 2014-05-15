#include <errno.h>
#include <gonggettext.h>
#include <gongfileutils.h>
#include "dbappdbapplication.h"
#include "dbappfrmdatabasetools.h"
#include "gongdbfielddefinition.h"

namespace gong {

class FrmPassword: public FrmCustom
{
public:
    FrmPassword( const Xtring &caption ) : FrmCustom()
    {
        setCaption( toGUI( caption ) );
        pEditPassword = addInput( 0, Xtring::printf( _("Introduzca la contraseña de la usuaria '%s'"),
                                  DBAPP->getDbUser().c_str() ), "PASSWORD", Xtring() );
    }
    Xtring getPassword() const {
        return pEditPassword->toString();
    }
private:
    LineEdit *pEditPassword;
};


FrmDatabaseTools::FrmDatabaseTools(QWidget* parent, const char* name, WidgetFlags fl)
    : FrmCustom(parent, name, fl)
{
    if( strempty(name) )
        setObjectName("FrmDatabaseTools");
    setCaption( "Herramientas de la base de datos" );
    pButtonUpgrade = addButton( this, _("Actualizar"), "buttonGrade" );
    pButtonCheck = addButton( this, _("Comprobar"), "buttonCheck" );
    pButtonBackup = addButton( this, _("Copia de seguridad"), "buttonBackup" );
    pButtonRestore = addButton( this, _("Recuperar copia"), "buttonRestore" );
    pButtonCheckDDD = addButton( this, _("Comprobar diccionario de datos"), "buttonCheckDDD" );
}

void FrmDatabaseTools::backupDatabase()
{
    DBAPP->waitCursor( true );
    FrmPassword *frmpassword = new FrmPassword( getTitle() );
    frmpassword->showModalFor( this, true );
    Xtring password = frmpassword->getPassword();
    delete frmpassword;
    if( !frmpassword->wasCancelled() ) {
        Xtring backupcommand = DBAPP->getSetting( "SYSTEM.COMMAND.DBBACKUP",
                               "mysqldump -u $user -p\"$password\" -h \"$host\" $database > \"$destfile\"" ).toString();
        Xtring origcommand = backupcommand;
        Xtring backupdir = DBAPP->getSetting( "SYSTEM.PATH.DBBACKUPS", DBAPP->getLocalDataDir() ).toString();
        Xtring fname = DBAPP->getSaveFileName( _("Elija el fichero de destino"),
                                               backupdir + "/" + DBAPP->getDatabase()->getName() + DateTime::currentDateTime().toString("%Y%m%d_%H%M") + ".sql",
                                               _("Copias de seguridad (*.sql);;Todos los archivos (*)" ),
                                               this );
        if ( !fname.isEmpty() ) {
            if ( FileUtils::extension ( fname ).isEmpty() )
                fname += ".sql";
            backupcommand.replace( "$user", DBAPP->getDbUser() );
            if( !DBAPP->getDbHost().isEmpty() )
                backupcommand.replace( "$host", DBAPP->getDbHost() );
            else
                backupcommand.replace( "$host", "localhost" );
            backupcommand.replace( "$password", password );
            backupcommand.replace( "$database", DBAPP->getDatabase()->getName() );
            backupcommand.replace( "$destfile", fname );
            Xtring errors;
            if( FileUtils::execProcess ( backupcommand.c_str(), Xtring::null, errors ) == -1 )
                errors = "No se ha podido ejecutar el comando: " + origcommand;
            if( !errors.isEmpty() ) {
                FrmBase::msgOk( this, _("Error"), errors );
            } else {
                FrmBase::msgOk( this, _("Completado"),
                                Xtring::printf(_("Se ha generado la copia de seguridad en el fichero\n%s"), fname.c_str()) );
            }
        }
    }
    DBAPP->resetCursor();
}

void FrmDatabaseTools::restoreDatabase()
{
    DBAPP->waitCursor( true );
    FrmPassword *frmpassword = new FrmPassword( getTitle() );
    frmpassword->showModalFor( this, true );
    Xtring password = frmpassword->getPassword();
    delete frmpassword;
    if( !frmpassword->wasCancelled() ) {
        Xtring restorecommand = DBAPP->getSetting( "SYSTEM.COMMAND.DBRESTORE",
                                "mysql -u $user -p\"$password\" -h \"$host\" $database < \"$origfile\"" ).toString();
        Xtring origcommand = restorecommand;
        Xtring backupdir = DBAPP->getSetting( "SYSTEM.PATH.DBBACKUPS", DBAPP->getLocalDataDir() ).toString();
        Xtring fname = DBAPP->getOpenFileName( _("Elija el fichero a restaurar"),
                                               backupdir,
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
                restorecommand.replace( "$user", DBAPP->getDbUser() );
                restorecommand.replace( "$host", host );
                restorecommand.replace( "$password", password );
                restorecommand.replace( "$database", DBAPP->getDatabase()->getName() );
                restorecommand.replace( "$origfile", fname );
                restorecommand += " 2>&1";
                Xtring errors;
                if( FileUtils::execProcess ( restorecommand.c_str(), Xtring::null, errors ) == -1 )
                    errors = "No se ha podido ejecutar el comando: " + origcommand;
                if( !errors.isEmpty() ) {
                    FrmBase::msgOk( this, _("Error"), errors );
                } else {
                    FrmBase::msgOk( this, _("Completado"),
                                    Xtring::printf(_("Se ha restaurado la copia de seguridad desde el fichero\n%s"), fname.c_str()) );
                }
            }
        }
    }
    DBAPP->resetCursor();
}

void FrmDatabaseTools::upgradeDatabase()
{
    DBAPP->waitCursor( true );
    Xtring diff = DBAPP->upgradeDatabase( DBAPP->getDatabase(), DBAPP->getDatabase()->getName(), true /*purging*/ );
    if( diff.isEmpty() )
        FrmBase::msgOk(this,  DBAPP->getTitle(), _( "La base de datos no necesitaba actualizarse" ) );
    DBAPP->resetCursor();
}

void FrmDatabaseTools::checkDatabase()
{
    DBAPP->waitCursor( true );
    try {
        DBAPP->showOSD( getTitle(), _("Comprobando índices"), 0 );
        DBAPP->getDatabase()->dropIndexes( DBAPP->getConnection(), true, true );
        DBAPP->getDatabase()->createIndexes( DBAPP->getConnection(), false );
        DBAPP->hideOSD();
        FrmBase::msgOk( this, _("Comprobando la base de datos"),
                        _("La base de datos se ha comprobado y está correcta") );
    } catch( dbError &e ) {
        DBAPP->hideOSD();
        FrmBase::msgOk(0, "Error comprobando la base de datos", e.getWholeError() );
    }
    DBAPP->resetCursor();
}

Xtring FrmDatabaseTools::checkDDDKey(const Xtring& key, const Xtring& missingvalue)
{
    Xtring ret;
    if( DBAPP->getSetting( key ).type() == Variant::tInvalid ) {
        ret = key + "= \"" + missingvalue + "\";\n";
    }
    return ret;
}

void FrmDatabaseTools::checkDDD()
{
    // Check fields that are not in the ddd
    DBAPP->waitCursor( true );
    Xtring ret, key;
    for( unsigned int nt = 0; nt < DBAPP->getDatabase()->getTables().size(); nt++ ) {
        const dbTableDefinition *tbldef = DBAPP->getDatabase()->getTableDefinition(nt);
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
    if( !ret.isEmpty() ) {
        FrmBase::msgOkLarge( this, getTitle(),
                             _("Estos campos no están definidos en el diccionario de datos"), ret );
    }
    DBAPP->resetCursor();
}

void FrmDatabaseTools::validateFields(QWidget* sender, bool* isvalid)
{
    if( sender == pButtonBackup )
        backupDatabase();
    else if( sender == pButtonCheck )
        checkDatabase();
    else if( sender == pButtonUpgrade )
        upgradeDatabase();
    else if( sender == pButtonRestore )
        restoreDatabase();
    else if( sender == pButtonCheckDDD )
        checkDDD();
}

} // namespace gong



