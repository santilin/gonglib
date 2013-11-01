#ifndef _GONG_DBAPPFRMLOGIN_H
#define _GONG_DBAPPFRMLOGIN_H

/** @file dbappfrmimport.h The form to log in to the application
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "dbappfrmlogin_gui.h"

namespace gong {

class dbError;

class FrmLogin : public GuiFrmLogin
{
    Q_OBJECT

public:
    enum LoginStatus {
        init_login,        // Login inicial
        error_autologin,   // No funcionó autologin
        error_nousuarios,  // No hay usuarios creados
        ok_dbcreada,       // La base de datos se ha creado
        ok       // Login correcto
    };
    FrmLogin(bool startingapp, QWidget* parent = 0, const char* name = 0,
             WidgetFlags fl = 0 );
    bool init(); // devuelve true si conecta automaticamente
    void addMessage(const Xtring &message);

public:
    Xtring getDriver() const { return mDriver; }
    void setDriver(const Xtring &driver) { mDriver = driver; }
    int getPort() const { return mPort; }
    void setPort(int port) { mPort = port; }
    Xtring getHost() const { return mHost; }
    void setHost(const Xtring &host) { mHost = host; }
    Xtring getUser() const { return mUser; }
    void setUser(const Xtring &user) { mUser = user; }
    Xtring getPassword() const { return mPassword; }
    void setPassword(const Xtring &password) { mPassword = password; }
    Xtring getDBName() const { return mDBName; }
    void setDBName(const Xtring &name) { mDBName = name; }
    bool isAutoLogin() const { return mAutoLogin; }
    void setAutoLogin(bool autologin) { mAutoLogin = autologin; }
    bool isSavePassword() const { return mSavePassword; }
    void setSavePassword(bool savepassword) { mSavePassword = savepassword; }
    Xtring getCreateDBDriver() const { return mCreateDBDriver; }
    void setCreateDBDriver(const Xtring &createdbdriver) { mCreateDBDriver = createdbdriver; }
    int getCreateDBPort() const { return mCreateDBPort; }
    void setCreateDBPort(int createdbport) { mCreateDBPort = createdbport; }
    Xtring getCreateDBHost() const { return mCreateDBHost; }
    void setCreateDBHost(const Xtring &createdbhost) { mCreateDBHost = createdbhost; }
    Xtring getCreateDBUser() const { return mCreateDBUser; }
    void setCreateDBUser(const Xtring &createdbuser) { mCreateDBUser = createdbuser; }
    Xtring getCreateDBPassword() const { return mCreateDBPassword; }
    void setCreateDBPassword(const Xtring &createdbpassword) { mCreateDBPassword = createdbpassword; }
    Xtring getCreateDBName() const { return mCreateDBName; }
    void setCreateDBName(const Xtring &createdbname) { mCreateDBName = createdbname; }
    Xtring getCreateUserUser() const { return mCreateUserUser; }
    void setCreateUserUser(const Xtring &createuseruser) { mCreateUserUser = createuseruser; }
    Xtring getCreateUserPassword() const { return mCreateUserPassword; }
    void setCreateUserPassword(const Xtring &createuserpassword) { mCreateUserPassword = createuserpassword; }
protected:
    Xtring mDriver;
    int mPort;
    Xtring mHost;
    Xtring mUser;
    Xtring mPassword;
    bool mAutoLogin;
    bool mSavePassword;
    Xtring mCreateDBDriver;
    int mCreateDBPort;
    Xtring mCreateDBHost;
    Xtring mCreateDBUser;
    Xtring mCreateDBPassword;
    Xtring mCreateDBName;
    Xtring mCreateUserUser;
    Xtring mCreateUserPassword;
    Xtring mCreateAsocUser;
    Xtring mCreateAsocPassword;
    Xtring mLoginWelcome;

protected slots:
    virtual void accept();
    void slotCreateDatabase();

private:
    dbError createDatabase();
    dbError createUser();
    int mGroupEstructuraOpcion;
    void updateConfig();
    void gather();
    void setMessages(const Xtring &welcome, const Xtring &detailed);
    Xtring getDBBaseName( const Xtring &composedname ) const;
    Xtring proposedDBName() const;
    Xtring mDBName;
    bool mStartingApp;
    LoginStatus mStatus;
};

} // namespace gong
#endif // _GONG_DBAPPFRMLOGIN_H
