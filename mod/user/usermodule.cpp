/*<<<<<MODULE_INFO*/
// COPYLEFT Módulo de usuarias y autenticación
// RECORD Usuaria FrmEditRecMaster Facturacion
// RECORD Rol FrmEditRecMaster Facturacion
// TYPE GongModule user::UserModule
/*>>>>>MODULE_INFO*/
/*<<<<<USERMODULE_INCLUDES*/
#include <QMenu>
#include <QMenuBar>
#include <gonggettext.h>
#include <dbappmainwindow.h>
#include <dbappdbapplication.h>
#include "usermodule.h"
#include "userfrmeditusuaria.h"
#include "userfrmeditrol.h"
/*>>>>>USERMODULE_INCLUDES*/

namespace gong {
namespace user {

UserModule *ModuleInstance = 0;

UserModule::UserModule()
	: dbModule("user")
{
	ModuleInstance = this;
    _GONG_DEBUG_TRACE(1);
/*<<<<<USERMODULE_PUBLIC_INFO*/
//	mModuleRequires
	mMasterTables << "USUARIA" << "ROL";
//	mDetailTables
/*>>>>>USERMODULE_PUBLIC_INFO*/
}


UserModule::~UserModule()
{
	_GONG_DEBUG_TRACE(1);
}


bool UserModule::initDatabase(dbDefinition *db)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT( db );
	pMainDatabase = db;

	pFicUser = new user::MasterTable( *pMainDatabase, "USER" );
    pFicUser->addFieldRecordID();
    pFicUser->addFieldStringCode("user", 100)->setUnique( true );
    pFicUser->addFieldStringCode("password", 64)->setUnique( true );
    pFicUser->addFieldBool( "active" )->setDefaultValue("1");
    pFicUser->addFieldNotas();
    pFicUser->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicUser->getTableDefinition() );

	
	
	return true;
}


/*<<<<<USERMODULE_SLOT_FACTURACIONUSER*/
void UserModule::slotMenuFacturacionUser()
{
	pMainWindow->slotMenuEditRecMaestro( "USER" );
}
/*>>>>>USERMODULE_SLOT_FACTURACIONUSER*/
/*<<<<<USERMODULE_SLOT_FACTURACIONROL*/
void UserModule::slotMenuFacturacionRol()
{
	pMainWindow->slotMenuEditRecMaestro( "ROL" );
}
/*>>>>>USERMODULE_SLOT_FACTURACIONROL*/
/*<<<<<USERMODULE_SLOT_FACTURACIONPERMISO*/
void UserModule::slotMenuFacturacionPermiso()
{
	pMainWindow->slotMenuEditRecMaestro( "PERMISO" );
}
/*>>>>>USERMODULE_SLOT_FACTURACIONPERMISO*/
/*<<<<<USERMODULE_INIT_DATABASE*/

/*>>>>>USERMODULE_INIT_DATABASE*/
dbRecord *UserModule::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
/*<<<<<USERMODULE_CREATE_RECORD*/
	if( tablename.upper() == "USUARIA" )
		return new RecUsuaria(getConnection(), recid, user);
	if( tablename.upper() == "ROL" )
		return new RecRol(getConnection(), recid, user);
/*>>>>>USERMODULE_CREATE_RECORD*/
	return 0;
}
FrmEditRec *UserModule::createEditForm(FrmEditRec *parentfrm, dbRecord *rec, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<USERMODULE_CREATE_EDITFORM*/
	if( tablename.upper() == "USUARIA" )
		return new FrmEditUsuaria(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
	if( tablename.upper() == "ROL" )
		return new FrmEditRol(parentfrm, rec, dm, editmode, editflags, parent, name, fl);
/*>>>>>USERMODULE_CREATE_EDITFORM*/
	return 0;
}

FrmEditRecDetail *UserModule::createEditDetailForm(
	FrmEditRecMaster *frmmaster, int ndetail,
	dbRecord *rec, const Xtring &dettablename, dbRecordDataModel *dm,
	FrmEditRec::EditMode editmode, dbApplication::EditFlags editflags,
	QWidget *parent, const char* name, WidgetFlags fl )
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	Xtring tablename = rec->getTableName();
/*<<<<<USERMODULE_CREATE_EDITFORM_DETAIL*/

/*>>>>>USERMODULE_CREATE_EDITFORM_DETAIL*/
	return 0;
}

bool UserModule::initMainWindow(MainWindow *mainwin)
{
	_GONG_DEBUG_ASSERT( ModuleInstance ); // Assign ModuleInstance to your application
	_GONG_DEBUG_ASSERT(mainwin);
	pMainWindow = mainwin;
/*<<<<<USERMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("USUARIA")->getDescPlural();
		pMenuFacturacionUsuaria = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuFacturacionUsuaria->setObjectName( "MenuFacturacionUsuaria" );
		pMenuFacturacionUsuaria->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuFacturacionUsuaria->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuFacturacionUsuaria, SIGNAL(activated()), this, SLOT(slotMenuFacturacionUsuaria()));
		pMenuFacturacionUsuaria->addTo(pMenuFacturacion);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("ROL")->getDescPlural();
		pMenuFacturacionRol = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuFacturacionRol->setObjectName( "MenuFacturacionRol" );
		pMenuFacturacionRol->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuFacturacionRol->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuFacturacionRol, SIGNAL(activated()), this, SLOT(slotMenuFacturacionRol()));
		pMenuFacturacionRol->addTo(pMenuFacturacion);
	}
/*>>>>>USERMODULE_INITMAINWINDOW_MENUS*/
	return true;
}

/*<<<<<USERMODULE_FIN*/
} // namespace user
} // namespace gong

/*>>>>>USERMODULE_FIN*/
/*<<<<<USERMODULE_SLOT_FACTURACIONUSUARIA*/
void UserModule::slotMenuFacturacionUsuaria()
{
	pMainWindow->slotMenuEditRecMaestro( "USUARIA" );
}
/*>>>>>USERMODULE_SLOT_FACTURACIONUSUARIA*/
