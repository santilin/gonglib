#ifndef _GONG_DBUSER_H
#define _GONG_DBUSER_H

/** @file gongdbuser.h Database user (unused at the moment)
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongxtring.h"
#include "gongdbdefinition.h"

namespace gong {

class dbUser {

public:
    enum UserType { user, guest, owner, admin };
    enum LogType { none, file, sql };
    ~dbUser();

    /*<<<<<GONGDBUSER_GETSET*/
public:
    dbUser(int usercode = 0, UserType usertype = user,
           const Xtring &logfilename = Xtring(), LogType logtype = none, bool logexecs = false,
           bool logselects = false, std::ofstream* logfile = 0,
           dbTableDefinition* logdatabase = 0, dbConnection *pConn = 0)
        :mUserCode(usercode), mUserType(usertype),
         mLogFilename(logfilename), mLogType(logtype), mLogExecs(logexecs),
         mLogSelects(logselects), pLogFile(logfile), mLogDatabase(logdatabase), pConnection(pConn)
    {};
    int getUserCode() const {
        return mUserCode;
    };
    void setUserCode(int usercode) {
        mUserCode = usercode;
    };
    UserType getUserType() const {
        return mUserType;
    };
    void setUserType(UserType usertype) {
        mUserType = usertype;
    };
    Xtring getLogFileName() const {
        return mLogFilename;
    };
    void setLogFileName(const Xtring &logfilename) {
        mLogFilename = logfilename;
    };
    LogType getLogType() const {
        return mLogType;
    };
    void setLogType(LogType logtype) {
        mLogType = logtype;
    };
    bool isLogExecs() const {
        return mLogExecs;
    };
    void setLogExecs(bool logexecs) {
        mLogExecs = logexecs;
    };
    bool isLogSelects() const {
        return mLogSelects;
    };
    void setLogSelects(bool logselects) {
        mLogSelects = logselects;
    };
    std::ofstream* getLogFile() const {
        return pLogFile;
    };
    void setLogFile(std::ofstream* logfile) {
        pLogFile = logfile;
    };
    dbTableDefinition* getLogDatabase() const {
        return mLogDatabase;
    };
    void setLogDatabase(dbTableDefinition* logdatabase) {
        mLogDatabase = logdatabase;
    };
protected:
    int mUserCode;
    UserType mUserType;
    Xtring mLogFilename;
    LogType mLogType;
    bool mLogExecs;
    bool mLogSelects;
    std::ofstream *pLogFile;
    dbTableDefinition* mLogDatabase;
    dbConnection *pConnection;
    /*>>>>>GONGDBUSER_GETSET*/

public:
    int doLog(const Xtring &message, int rowsaffected=-1);
};

}; // namespace gong


#endif // GONGDBUSER_H
