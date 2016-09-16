/*<<<<<COPYLEFT*/
/** @file gestiongmodule.cpp
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
// TYPE GongModule gestiong::GestiongModule
/*>>>>>MODULE_INFO*/
/*<<<<<GESTIONGMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "gestiongmodule.h"
/*>>>>>GESTIONGMODULE_INCLUDES*/

namespace gong {
namespace gestiong {

GestiongModule *GestiongModuleApp = 0;

GestiongModule::GestiongModule()
    : dbModule("gestiong")
{
    GestiongModuleApp = this;
    _GONG_DEBUG_PRINT(1, "Inicializando GestiongModule...");
    /*<<<<<GESTIONGMODULE_PUBLIC_INFO*/
//	mModuleRequires
//	mMasterTables
//	mDetailTables
/*>>>>>GESTIONGMODULE_PUBLIC_INFO*/
}


GestiongModule::~GestiongModule()
{
    _GONG_DEBUG_PRINT(1, "Destruyendo module GestiongModule...");
}

bool GestiongModule::login( QDialog *frmlogin, const Xtring& version,
                           Xtring& addTitle, bool startingapp)
{
    return true;
}

bool GestiongModule::initDatabase(dbDefinition *db)
{
    _GONG_DEBUG_ASSERT( GestiongModuleApp ); // Assign GestiongModuleApp to your application
    _GONG_DEBUG_ASSERT( db );
    pMainDatabase = db;

    return true;
}
dbRecord *GestiongModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
    _GONG_DEBUG_ASSERT( GestiongModuleApp ); // Assign GestiongModuleApp to your application
    /*<<<<<GESTIONGMODULE_CREATE_RECORD*/

/*>>>>>GESTIONGMODULE_CREATE_RECORD*/
    return 0;
}
FrmEditRec *GestiongModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
        FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
        QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( GestiongModuleApp ); // Assign GestiongModuleApp to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<GESTIONGMODULE_CREATE_EDITFORM*/

/*>>>>>GESTIONGMODULE_CREATE_EDITFORM*/
    return 0;
}

FrmEditRecDetail *GestiongModule::createEditDetailForm(
    FrmEditRecMaster *frmmaster, int ndetail,
    dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
    FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
    QWidget *parent, const char* name, WidgetFlags fl )
{
    _GONG_DEBUG_ASSERT( GestiongModuleApp ); // Assign GestiongModuleApp to your application
    Xtring tablename = rec->getTableName();
    /*<<<<<GESTIONGMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>GESTIONGMODULE_CREATE_EDITFORM_DETAIL*/
    return 0;
}

bool GestiongModule::initMainWindow(MainWindow *mainwin)
{
    _GONG_DEBUG_ASSERT( GestiongModuleApp ); // Assign GestiongModuleApp to your application
    _GONG_DEBUG_ASSERT(mainwin);
    pMainWindow = mainwin;
    /*<<<<<GESTIONGMODULE_INITMAINWINDOW_MENUS*/

/*>>>>>GESTIONGMODULE_INITMAINWINDOW_MENUS*/
    return true;
}
/*<<<<<GESTIONGMODULE_FIN*/
} // namespace gestiong
} // namespace gong

/*>>>>>GESTIONGMODULE_FIN*/

/*<<<<<GESTIONGMODULE_INIT_DATABASE*/

/*>>>>>GESTIONGMODULE_INIT_DATABASE*/
