/*<<<<<COPYLEFT*/
/** @file reccolormodule.cpp Module RecColor
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

/*<<<<<MODULE_INFO*/
// COPYLEFT Module RecColor
// TYPE GongModule reccolor::RecColorModule
/*>>>>>MODULE_INFO*/

/*<<<<<RECCOLORMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "reccolormodule.h"
/*>>>>>RECCOLORMODULE_INCLUDES*/
#include "reccolorrecordbehavior.h"
#include "reccolorfrmeditrecbehavior.h"

namespace gong {
namespace reccolor {

RecColorModule *ModuleInstance = 0;
static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::StringList,
        "TABLES",
        _("Tablas a colorear"),
        ""
    },
    { dbModuleSetting::Int, "COLORNAME_1", _("Nombre del color 1"), "Color 1" },
    { dbModuleSetting::Int, "COLORNAME_2", _("Nombre del color 2"), "Color 2" },
    { dbModuleSetting::Int, "COLORNAME_3", _("Nombre del color 3"), "Color 3" },
    { dbModuleSetting::Int, "COLORNAME_4", _("Nombre del color 4"), "Color 4" },
    { dbModuleSetting::Int, "COLORNAME_5", _("Nombre del color 5"), "Color 5" },
    { dbModuleSetting::Int, "COLORNAME_6", _("Nombre del color 6"), "Color 6" },
    { dbModuleSetting::Int, "COLORNAME_7", _("Nombre del color 7"), "Color 7" },
    { dbModuleSetting::Int, "COLORNAME_8", _("Nombre del color 8"), "Color 8" },
    { dbModuleSetting::Int, "COLORNAME_9", _("Nombre del color 9"), "Color 9" },
    { dbModuleSetting::Int, "COLORNAME_10", _("Nombre del color 10"), "Color 10" },
    { dbModuleSetting::Int, "COLORNAME_11", _("Nombre del color 11"), "Color 11" },
    { dbModuleSetting::Int, "COLORNAME_12", _("Nombre del color 12"), "Color 12" },
    { dbModuleSetting::Int, "COLORNAME_13", _("Nombre del color 13"), "Color 13" },
    { dbModuleSetting::Int, "COLORNAME_14", _("Nombre del color 14"), "Color 14" },
    { dbModuleSetting::Int, "COLORNAME_15", _("Nombre del color 15"), "Color 15" },
    { dbModuleSetting::None}
};


RecColorModule::RecColorModule()
    : dbModule("reccolor")
{
    ModuleInstance = this;
    pModuleSettings = _settings;
    _GONG_DEBUG_TRACE(1);
    /*<<<<<RECCOLORMODULE_PUBLIC_INFO*/
//	mModuleRequires
//	mMasterTables
//	mDetailTables
/*>>>>>RECCOLORMODULE_PUBLIC_INFO*/
    pColorRecordBehavior = new ColorRecordBehavior();
}

RecColorModule::~RecColorModule()
{
    _GONG_DEBUG_TRACE(1);
}


bool RecColorModule::initDatabase(dbDefinition *db)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

    return true;
}


dbRecord *RecColorModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    /*<<<<<RECCOLORMODULE_CREATE_RECORD*/

/*>>>>>RECCOLORMODULE_CREATE_RECORD*/
    return 0;
}

FrmEditRec *RecColorModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<RECCOLORMODULE_CREATE_EDITFORM*/

/*>>>>>RECCOLORMODULE_CREATE_EDITFORM*/
    return 0;
}

FrmEditRecDetail *RecColorModule::createEditDetailForm(FrmEditRecMaster *frmmaster,
        int ndetail, dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
        FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<RECCOLORMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>RECCOLORMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}

void RecColorModule::afterLoad()
{
}


bool RecColorModule::login(FrmLogin *frmlogin, const Xtring& version,
                           Xtring& addTitle, bool startingapp)
{
    Xtring cfg_tables = DBAPP->getAppSettingConcat( "MODULE.RECCOLOR.TABLES", "" );
    cfg_tables.tokenize(mTables, ",");
    _GONG_DEBUG_PRINT(0, cfg_tables );
    for( XtringList::const_iterator it = mTables.begin(); it != mTables.end(); ++it ) {
        Xtring table = *it;
        dbTableDefinition *tbldef = getDatabase()->findTableDefinition( table );
        if( tbldef ) {
            tbldef->addBehavior( pColorRecordBehavior );
        }
    }
    return true;
}


bool RecColorModule::initMainWindow(MainWindow *mainwin)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT(mainwin);
    pMainWindow = mainwin;
    /*<<<<<RECCOLORMODULE_INITMAINWINDOW_MENUS*/

/*>>>>>RECCOLORMODULE_INITMAINWINDOW_MENUS*/
    return true;
}


void RecColorModule::afterCreateEditForm(FrmEditRec* frm, dbRecord *rec)
{
    _GONG_DEBUG_TRACE(3);
    for( XtringList::const_iterator it = mTables.begin(); it != mTables.end(); ++it ) {
        Xtring table = *it;
        if( table == rec->getTableName() ) {
            frm->addBehavior( new ColorFrmEditRecBehavior( frm ) );
            break;
        }
    }
}


/*<<<<<RECCOLORMODULE_FIN*/
} // namespace reccolor
} // namespace gong

/*>>>>>RECCOLORMODULE_FIN*/
/*<<<<<RECCOLORMODULE_INIT_DATABASE*/

/*>>>>>RECCOLORMODULE_INIT_DATABASE*/
