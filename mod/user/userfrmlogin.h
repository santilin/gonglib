#ifndef _GONG_USERFRMLOGIN_H
#define _GONG_USERFRMLOGIN_H

/** @file userfrmlogin.h Login del módulo de usuarias
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <dbappfrmcustom.h>

namespace gong {
namespace user {

class FrmLogin: public FrmCustom
{
public:
    FrmLogin(QWidget* parent = 0);

    Xtring getUser() const { return editUser->toString(); }
    void setUser(const Xtring &user) { editUser->setText(user); }
    Xtring getPassword() const { return editPassword->toString(); }
    void setPassword(const Xtring &password) { editPassword->setText(password); }
    bool isAutoLogin() const { return checkAutoLogin->isChecked(); }
    void setAutoLogin(bool autologin) { checkAutoLogin->setChecked(autologin); }
    bool isSavePassword() const { return checkSavePassword->isChecked(); }
    void setSavePassword(bool savepassword) { checkSavePassword->setChecked(savepassword); }

protected:
    LineEdit *editUser;
    LineEdit *editPassword;
	CheckBox *checkAutoLogin;
	CheckBox *checkSavePassword;
	bool mIsLogged;
	// Para DialogCustom
    QWidget* pControlsFrame;
    QVBoxLayout *pFormLayout, *pMainLayout, *pControlsLayout;
    QHBoxLayout *pButtonsLayout;

};

} // namespace gong
} // namespace user
#endif // _GONG_DBAPPFRMLOGIN_H
