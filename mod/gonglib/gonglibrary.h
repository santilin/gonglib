#ifndef _GONG_LIBRARY_H
#define _GONG_LIBRARY_H

/** @file gonglibrary.h Global initialization of the gong library
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongregconfig.h"
#include "gongdictionary.h"
#include "gongdbconnection.h"

namespace gong {

class dbRecord;
class Settings;

class GongLibrary
{
public:
    GongLibrary(const char *datadir, const char *packagename, const char *packageversion,
                int argc=0, char *argv[]=0);
    virtual ~GongLibrary();
    virtual void init();

    RegConfig *getRegConfig() const { return const_cast<RegConfig *>(&mRegConfig); }
    const Xtring &getLocalDataDir() const { return mLocalDataDir; }
    const Xtring &getGlobalDataDir() const {
        return mGlobalDataDir;
    }
    const Xtring &getGonglibDataDir() const {
        return mGonglibDataDir;
    }
    const char *getPackageName() const {
        return mPackageName;
    }
    const char *getPackageVersion() const {
        return mPackageVersion;
    }
    Xtring getPackageString() const {
        return Xtring(mPackageName) + " " + mPackageVersion;
    }
    virtual dbRecord *createRecord(const Xtring &tablename, dbRecordID recid=0, class dbUser *user=0);
    Xtring getHostIP(const Xtring &host) const;
    Xtring getInterfaceIP(const Xtring &ifname) const;
    Xtring getUsage() const;
	
protected:
    int parseArguments( int argc, char **argv );
	
private:
    Xtring mLocalDataDir, mGlobalDataDir, mGonglibDataDir;
    const char *mPackageName, *mPackageVersion;
    RegConfig mRegConfig;
    Settings *pSettings;
};

extern const IntList NullIntList;
extern IntList IdentityIntList;
extern const XtringList NullXtringList;

extern GongLibrary *GongLibraryInstance;

}; // namespace gong

#endif // _GONG_LIBRARY_H
