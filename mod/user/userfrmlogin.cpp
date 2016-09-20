#include <dbappdbapplication.h>
#include "usermodule.h"
#include "userfrmlogin.h"

namespace gong {
namespace user {

FrmLogin::FrmLogin(QWidget* parent)
    : FrmCustom(parent)
{
    setCaption( toGUI("Acceso a " + DBAPP->getPackageString() ) );
	editUser = addInputField( 0, Xtring::null, "USUARIA", "LOGIN", DBAPP->getAppSetting("USER").toString());
	editPassword = addInputField( 0, Xtring::null, "USUARIA", "PASSWORD", DBAPP->getAppSetting("PASSWORD").toString());
	checkSavePassword = addCheckBox( this, _("Guardar contraseña"),
									   DBAPP->getAppSetting ( "SAVEPASSWORD", false ).toBool());
	checkAutoLogin = addCheckBox( this, _("Entrar sin preguntar"),
									DBAPP->getAppSetting ( "AUTOLOGIN", false ).toBool());
}



} // namespace user
} // namespace gong
