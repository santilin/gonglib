/*<<<<<COPYLEFT*/
/** @file sinpapelesmodule.cpp Module SinPapeles
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
// COPYLEFT Module SinPapeles
// TYPE GongModule sinpapeles::SinPapelesModule
/*>>>>>MODULE_INFO*/

/*<<<<<SINPAPELESMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "sinpapelesmodule.h"
/*>>>>>SINPAPELESMODULE_INCLUDES*/
#include "sinpapelesrecordbehavior.h"
#include "sinpapelesfrmeditrecbehavior.h"

namespace gong {
namespace sinpapeles {

SinPapelesModule *ModuleInstance = 0;
static dbModuleSetting _settings[] = {
    {
        dbModuleSetting::String, // TODO StringList
        "TABLES",
        _("Tablas a que guardarán documentos escaneados"),
        ""
    },
    {
        dbModuleSetting::StringList,
        "OPEN_COMMAND",
        _("Comando del sistema operativo para abrir documentos"),
        ""
    },
    { dbModuleSetting::None}
};

SinPapelesModule::SinPapelesModule()
    : dbModule("sinpapeles")
{
    ModuleInstance = this;
    pModuleSettings = _settings;
    _GONG_DEBUG_TRACE(1);
    /*<<<<<SINPAPELESMODULE_PUBLIC_INFO*/
//	mModuleRequires
//	mMasterTables
//	mDetailTables
/*>>>>>SINPAPELESMODULE_PUBLIC_INFO*/
    pSinPapelesRecordBehavior = new SinPapelesRecordBehavior();
}

SinPapelesModule::~SinPapelesModule()
{
    _GONG_DEBUG_TRACE(1);
}


bool SinPapelesModule::initDatabase(dbDefinition *db)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

    Xtring cfg_tables = ModuleInstance->getModuleSetting( "TABLES" ).toString();
    cfg_tables.tokenize(mTables, ",");
    _GONG_DEBUG_PRINT(0, cfg_tables );
    for( XtringList::const_iterator it = mTables.begin(); it != mTables.end(); ++it ) {
        Xtring table = *it;
        dbTableDefinition *tbldef = getDatabase()->findTableDefinition( table );
        if( tbldef ) {
            tbldef->addBehavior( pSinPapelesRecordBehavior );
        }
    }
    return true;
}


dbRecord *SinPapelesModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    /*<<<<<SINPAPELESMODULE_CREATE_RECORD*/

/*>>>>>SINPAPELESMODULE_CREATE_RECORD*/
    return 0;
}

FrmEditRec *SinPapelesModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<SINPAPELESMODULE_CREATE_EDITFORM*/

/*>>>>>SINPAPELESMODULE_CREATE_EDITFORM*/
    return 0;
}

FrmEditRecDetail *SinPapelesModule::createEditDetailForm(
    FrmEditRecMaster *frmmaster, int ndetail,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<SINPAPELESMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>SINPAPELESMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}

void SinPapelesModule::afterLoad()
{
}


bool SinPapelesModule::login( QWidget *frmlogin, const Xtring& version,
                             Xtring& addTitle, bool startingapp)
{
    return true;
}


bool SinPapelesModule::initMainWindow(MainWindow *mainwin)
{
    _GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
    _GONG_DEBUG_ASSERT(mainwin);
    pMainWindow = mainwin;
    /*<<<<<SINPAPELESMODULE_INITMAINWINDOW_MENUS*/

/*>>>>>SINPAPELESMODULE_INITMAINWINDOW_MENUS*/
    return true;
}


void SinPapelesModule::afterCreateEditForm(FrmEditRec* frm, dbRecord *rec)
{
    _GONG_DEBUG_TRACE(3);
    for( XtringList::const_iterator it = mTables.begin(); it != mTables.end(); ++it ) {
        Xtring table = *it;
        if( table == rec->getTableName() ) {
            frm->addBehavior( new SinPapelesFrmEditRecBehavior( frm ) );
            break;
        }
    }
}


/*<<<<<SINPAPELESMODULE_FIN*/
} // namespace sinpapeles
} // namespace gong

/*>>>>>SINPAPELESMODULE_FIN*/
/*<<<<<SINPAPELESMODULE_INIT_DATABASE*/

/*>>>>>SINPAPELESMODULE_INIT_DATABASE*/
