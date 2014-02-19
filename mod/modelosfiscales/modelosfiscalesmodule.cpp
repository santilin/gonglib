/*<<<<<MODULE_INFO*/
// COPYLEFT Module modelosfiscales
// TYPE GongModule modelosfiscales::ModelosFiscalesModule
/*>>>>>MODULE_INFO*/

/*<<<<<MODELOSFISCALESMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "modelosfiscalesmodule.h"
/*>>>>>MODELOSFISCALESMODULE_INCLUDES*/
ModelosFiscalesModule::ModelosFiscalesModule()
	: dbModule("modelosfiscales")
{
	ModuleInstance = this;
    _GONG_DEBUG_TRACE(1);
/*<<<<<MODELOSFISCALESMODULE_PUBLIC_INFO*/
//	mModuleRequires
//	mMasterTables
//	mDetailTables
/*>>>>>MODELOSFISCALESMODULE_PUBLIC_INFO*/
}


ModelosFiscalesModule::~ModelosFiscalesModule()
{
	_GONG_DEBUG_TRACE(1);
}


bool ModelosFiscalesModule::initDatabase(dbDefinition *db)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT( db );
	pMainDatabase = db;
/*<<<<<MODELOSFISCALESMODULE_INIT_DATABASE*/

/*>>>>>MODELOSFISCALESMODULE_INIT_DATABASE*/

    pFicFacturaVenta->addFieldDate( "FECHAIVA" );
    pFicFacturaCompra->addFieldDate( "FECHAIVA" );
	pFicArticulo->addFieldFloat("PORCENTAJEIVA" );

	return true;
}

void FactuModule::slotMenuFactuMod303()
{
    FrmMod303 *frm303 = new FrmMod303 ();
    frm303->show();
    pMainWindow->createClient ( frm303 );
}

dbRecord *ModelosFiscalesModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
/*<<<<<MODELOSFISCALESMODULE_CREATE_RECORD*/

/*>>>>>MODELOSFISCALESMODULE_CREATE_RECORD*/
	return 0;
}

FrmEditRec *ModelosFiscalesModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<MODELOSFISCALESMODULE_CREATE_EDITFORM*/

/*>>>>>MODELOSFISCALESMODULE_CREATE_EDITFORM*/
	return 0;
}

FrmEditRecDetail *ModelosFiscalesModule::createEditDetailForm(
	FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<MODELOSFISCALESMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>MODELOSFISCALESMODULE_CREATE_EDITFORM_DETAIL*/
	return 0;
}

bool ModelosFiscalesModule::initMainWindow(MainWindow *mainwin)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT(mainwin);
	pMainWindow = mainwin;
/*<<<<<MODELOSFISCALESMODULE_INITMAINWINDOW_MENUS*/

/*>>>>>MODELOSFISCALESMODULE_INITMAINWINDOW_MENUS*/
	return true;
}

/*<<<<<MODELOSFISCALESMODULE_FIN*/
} // namespace modelosfiscales
} // namespace gong

/*>>>>>MODELOSFISCALESMODULE_FIN*/
