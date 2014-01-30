/*<<<<<MODULE_INFO*/
// COPYLEFT Module ecotienda
// TYPE GongModule ecotienda::EcoTiendaModule
/*>>>>>MODULE_INFO*/
/*<<<<<ECOTIENDAMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "ecotiendamodule.h"
/*>>>>>ECOTIENDAMODULE_INCLUDES*/
EcoTiendaModule::EcoTiendaModule()
	: dbModule("ecotienda")
{
	ModuleInstance = this;
    _GONG_DEBUG_TRACE(1);
/*<<<<<ECOTIENDAMODULE_PUBLIC_INFO*/
//	mModuleRequires
//	mMasterTables
//	mDetailTables
/*>>>>>ECOTIENDAMODULE_PUBLIC_INFO*/
}


EcoTiendaModule::~EcoTiendaModule()
{
	_GONG_DEBUG_TRACE(1);
}


bool EcoTiendaModule::initDatabase(dbDefinition *db)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT( db );
	pMainDatabase = db;

	return true;
}

namespace gong {
namespace ecotienda {

EcoTiendaModule *ModuleInstance = 0;

/*<<<<<ECOTIENDAMODULE_INIT_DATABASE*/

/*>>>>>ECOTIENDAMODULE_INIT_DATABASE*/
dbRecord *EcoTiendaModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
/*<<<<<ECOTIENDAMODULE_CREATE_RECORD*/

/*>>>>>ECOTIENDAMODULE_CREATE_RECORD*/
	return 0;
}
FrmEditRec *EcoTiendaModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<ECOTIENDAMODULE_CREATE_EDITFORM*/

/*>>>>>ECOTIENDAMODULE_CREATE_EDITFORM*/
	return 0;
}

FrmEditRecDetail *EcoTiendaModule::createEditDetailForm(
	FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<ECOTIENDAMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>ECOTIENDAMODULE_CREATE_EDITFORM_DETAIL*/
	return 0;
}

bool EcoTiendaModule::initMainWindow(MainWindow *mainwin)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT(mainwin);
	pMainWindow = mainwin;
/*<<<<<ECOTIENDAMODULE_INITMAINWINDOW_MENUS*/

/*>>>>>ECOTIENDAMODULE_INITMAINWINDOW_MENUS*/
	return true;
}
/*<<<<<ECOTIENDAMODULE_FIN*/
} // namespace ecotienda
} // namespace gong

/*>>>>>ECOTIENDAMODULE_FIN*/
