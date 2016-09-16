/*<<<<<MODULE_INFO*/
// COPYLEFT Módulo de usuarias y autenticación
// RECORD Usuaria FrmEditRecMaster System
// RECORD Rol FrmEditRecMaster System
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
#include "userfrmlogin.h"

namespace gong {
namespace user {

UserModule *ModuleInstance = 0;

UserModule::UserModule()
	: dbModule("user"), mIsLogged(false)
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

	/*<<<<<USERMODULE_INIT_DATABASE*/

/*>>>>>USERMODULE_INIT_DATABASE*/

	pFicUsuaria = new user::MasterTable( *pMainDatabase, "USUARIA" );
    pFicUsuaria->addFieldRecordID();
    pFicUsuaria->addFieldStringCode("LOGIN", 100)->setUnique( true );
    pFicUsuaria->addFieldStringCode("PASSWORD", 64)->setUnique( true );
    pFicUsuaria->addFieldBool( "ACTIVA" )->setDefaultValue("1");
    pFicUsuaria->addFieldNotas();
    pFicUsuaria->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicUsuaria->getTableDefinition() );

	pFicRol = new user::MasterTable( *pMainDatabase, "ROL" );
    pFicRol->addFieldRecordID();
    pFicRol->addFieldStringCode("NOMBRE", 100)->setUnique( true );
    pFicRol->addFieldNotas();
    pFicRol->addBehavior( DBAPP->getRecordTimestampBehavior() );
    pMainDatabase->addTable( pFicRol->getTableDefinition() );
	
	
	return true;
}

/*<<<<<USERMODULE_SLOT_SYSTEMUSUARIA*/
void UserModule::slotMenuSystemUsuaria()
{
	pMainWindow->slotMenuEditRecMaestro( "USUARIA" );
}
/*>>>>>USERMODULE_SLOT_SYSTEMUSUARIA*/
/*<<<<<USERMODULE_SLOT_SYSTEMROL*/
void UserModule::slotMenuSystemRol()
{
	pMainWindow->slotMenuEditRecMaestro( "ROL" );
}
/*>>>>>USERMODULE_SLOT_SYSTEMROL*/

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
	QMenu *pMenuSystem = mainwin->getMenuSystem();
	
/*<<<<<USERMODULE_INITMAINWINDOW_MENUS*/
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("USUARIA")->getDescPlural();
		pMenuSystemUsuaria = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuSystemUsuaria->setObjectName( "MenuSystemUsuaria" );
		pMenuSystemUsuaria->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuSystemUsuaria->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuSystemUsuaria, SIGNAL(activated()), this, SLOT(slotMenuSystemUsuaria()));
		pMenuSystemUsuaria->addTo(pMenuSystem);
	}
	{
		Xtring caption = DBAPP->getDatabase()->findTableDefinition("ROL")->getDescPlural();
		pMenuSystemRol = new QAction( toGUI( caption ) + "...", pMainWindow );
		pMenuSystemRol->setObjectName( "MenuSystemRol" );
		pMenuSystemRol->setStatusTip( toGUI( Xtring::printf( _("Fichero de %s"), caption.c_str() ) ) );
		pMenuSystemRol->setWhatsThis( toGUI( Xtring::printf( _("Abre el fichero de "), caption.c_str() ) ) );
		pMainWindow->connect(pMenuSystemRol, SIGNAL(activated()), this, SLOT(slotMenuSystemRol()));
		pMenuSystemRol->addTo(pMenuSystem);
	}
/*>>>>>USERMODULE_INITMAINWINDOW_MENUS*/
	return true;
}

bool UserModule::login(gong::FrmLogin *frmlogin, const Xtring &version, Xtring &addTitle, bool startingapp)
{
	bool autologin = DBAPP->getAppSetting ( "AUTOLOGIN", "false" ).toBool();
	bool logged = false;
	if (autologin) {
		logged = doLogin(DBAPP->getAppSetting("USER").toString(), DBAPP->getAppSetting("PASSWORD").toString());
	}
	if( !logged) {
		FrmLogin *loginform = new FrmLogin();
		loginform->exec();
		if (loginform->result() == QDialog::Accepted)
			logged = doLogin( loginform->getUser(), loginform->getPassword() );
	}
	return logged;
}

bool UserModule::doLogin(const Xtring& user, const Xtring& password)
{
	Xtring r( pConnection->selectString("SELECT LOGIN FROM USUARIA WHERE"
	"LOGIN=" + pConnection->toSQL(user) + " AND PASSWORD=" + pConnection->toSQL(password)));
	return !r.isEmpty();
}



/*<<<<<USERMODULE_FIN*/
} // namespace user
} // namespace gong

/*>>>>>USERMODULE_FIN*/


