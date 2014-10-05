#include <QAction>
#include <gonggettext.h>
#include <gongfileutils.h>
#include "dbappmainwindow.h"
#include "dbappdbapplication.h"
#include "dbappdbmodule.h"

namespace gong {

dbModule::dbModule(const char *unixname)
    : pApplication(DBAPP), pMainWindow(0), pMainDatabase(0), pConnection(0),
      mVersion(1), mUnixName( unixname ), pModuleSettings(0), mEnabled( true )
{
    mModuleGlobalDataDir = pApplication->getGonglibDataDir() + mUnixName + "/";
    mModuleLocalDataDir = pApplication->getLocalDataDir() + mUnixName + "/";
}


dbConnection *dbModule::getConnection() const
{
    return( pConnection ? pConnection : pApplication->getConnection() );
}

/**
 * @brief Reads this module's settings
 *        This is called once for each module in the main \a dbApplication::readSettings
 *        Several sets of settings are read:
 *        * settings.rc: The global settings for the module. There are no local settings for modules
 *        * stylesheets.css: The global and local stylessheet.
 *        * [modulename].ddd .- Database dictionary data, both local and global,
 *              they can be localized if the language code is prepended to the modulename: es_module.ddd
 *	      Also, if there are reports paths, they are added to \a dbApplication::sReportsPath
 **/
void dbModule::readSettings()
{
    _GONG_DEBUG_PRINT(1, "Reading settings for module " + mUnixName );
    DBAPP->readMachineSettings( mModuleGlobalDataDir + mUnixName + ".ddd" );
// 	// Read localized ddd
// 	_GONG_DEBUG_PRINT(3, "Reading localized " + DBAPP->getRegConfig()->getLanguage() + " ddd for module " + mUnixName
// 		+ ":" + mModuleGlobalDataDir + DBAPP->getRegConfig()->getLanguage() + "_" + mUnixName + ".ddd" );
// 	if( FileUtils::exists( (mModuleGlobalDataDir
//                 + DBAPP->getRegConfig()->getLanguage() + "_" + mUnixName + ".ddd").c_str() ) ) {
// 		DBAPP->readMachineSettings( mModuleGlobalDataDir
//                         + DBAPP->getRegConfig()->getLanguage() + "_" + mUnixName + ".ddd" );
// 	}
    if( FileUtils::exists( (mModuleGlobalDataDir + "settings.rc").c_str() ) )
        DBAPP->readMachineSettings( mModuleGlobalDataDir + "settings.rc" );
    if( FileUtils::exists( (mModuleGlobalDataDir + "informes").c_str() ) ) {
        DBAPP->addReportsPath( false,  mModuleGlobalDataDir + "informes/" );
        _GONG_DEBUG_PRINT(2, "Adding rtkpath: " + mModuleGlobalDataDir + "informes/" );
    } else {
        _GONG_DEBUG_PRINT(3, "Not adding rtkpath: " + mModuleGlobalDataDir + "informes" );
    }
    // There are no local settings for modules, they are all stored in the local settings file for the application
    if( FileUtils::exists( (mModuleLocalDataDir + "informes").c_str() ) ) {
        DBAPP->addReportsPath( true, mModuleLocalDataDir + "informes/" );
        _GONG_DEBUG_PRINT(2, "Adding rtkpath: " + mModuleLocalDataDir + "informes/" );
    } else {
        _GONG_DEBUG_PRINT(3, "Not adding rtkpath: " + mModuleLocalDataDir + "informes" );
    }
    QString ss( DBAPP->styleSheet() );
    bool ssread = false;
    if( FileUtils::exists( (mModuleGlobalDataDir + "stylesheet.css").c_str() ) ) {
        ss += FileUtils::readFile( mModuleGlobalDataDir + "stylesheet.css" ).c_str();
        ssread = true;
        _GONG_DEBUG_PRINT(2, "Adding stylesheet: " + mModuleGlobalDataDir + "stylesheet.css" );
    } else {
        _GONG_DEBUG_PRINT(3, "NOT ADDING stylesheet: " + mModuleGlobalDataDir + "stylesheet.css" );
    }
    if( FileUtils::exists( (mModuleLocalDataDir + "stylesheet.css").c_str() ) ) {
        ss += FileUtils::readFile( mModuleLocalDataDir + "stylesheet.css" ).c_str();
        ssread = true;
        _GONG_DEBUG_PRINT(2, "Adding stylesheet: " + mModuleLocalDataDir + "stylesheet.css" );
    } else {
        _GONG_DEBUG_PRINT(3, "NOT ADDING stylesheet: " + mModuleLocalDataDir + "stylesheet.css" );
    }
    if( ssread )
        DBAPP->setStyleSheet( ss );
}

Variant dbModule::getModuleSetting(const Xtring& settingname) const
{
    Variant defaultvalue;
    const dbModuleSetting *ms = getModuleSettings();
    while( ms && ms->type != dbModuleSetting::None ) {
        if( strcmp( ms->key, settingname.c_str() ) == 0 ) {
            defaultvalue = ms->defaultvalue;
            break;
        }
        ms++;
    }
    return DBAPP->getAppSetting( "MODULE." + mUnixName.upper() + "." + settingname, defaultvalue );
}

QAction* dbModule::createActionForTable(const Xtring& tablename, const Xtring &shortcut)
{
    QAction *act_menu;
    Xtring caption = getDatabase()->findTableDefinition(tablename)->getDescPlural();
    act_menu = new QAction( toGUI(caption + "..."), pMainWindow );
    act_menu->setStatusTip( toGUI( _("Fichero de ") + caption) );
    act_menu->setWhatsThis( toGUI( _("Abre el fichero de ") + caption) );
    if( !shortcut.isEmpty() )
        act_menu->setShortcut( QKeySequence( toGUI(shortcut) ) );
    return act_menu;
}

Variant dbModule::getGlobalProperty(const Xtring& propertyname) const
{
    return Variant();
}


} // namespace


