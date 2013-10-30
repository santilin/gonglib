#ifndef _GONG_DBSETTINGS_H
#define _GONG_DBSETTINGS_H

/** @file gongdbsettings.h Read/Write application settings stored in a database
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongdbconnection.h"
#include "gongsettings.h"

namespace gong {

class dbSettings: public Settings
{
public:
    dbSettings(dbConnection *dbconn, const Xtring &tablename,
               const Xtring &fldkeyname, const Xtring &fldvaluename,
               const Xtring &fldtypename, const Xtring &fldusername,
               dbRecordID user_id = 0, const Xtring &desc = Xtring::null)
        : Settings( tablename, desc ), pConnection( dbconn ), mUserID( user_id ),
          mTableName(tablename), mFldKeyName(fldkeyname), mFldValueName(fldvaluename),
          mFldTypeName(fldtypename), mFldUserName(fldusername), mDescription( desc ) {}

    virtual bool read(const Xtring &select = Xtring::null); // from Settings
    virtual bool write(const Xtring &update = Xtring::null); // from Settings
    virtual void setValue(const Xtring &settingname, const Variant &val); // from Settings
    virtual void removeSetting(const Xtring &settingname); // from Settings

private:
    dbConnection *pConnection;
    dbRecordID mUserID;
    Xtring mTableName, mFldKeyName, mFldValueName, mFldTypeName, mFldUserName, mDescription;
};

} // namespace gong
#endif // _GONG_SETTINGS_H
