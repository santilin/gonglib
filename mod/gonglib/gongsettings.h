#ifndef _GONG_SETTINGS_H
#define _GONG_SETTINGS_H

/** @file gongsettings.cpp Read/Write application settings
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
#include "gongvariant.h"
#include "gongdictionary.h"

namespace gong {

typedef Dictionary<Variant> SettingsDict;

class Settings
{
public:
    Settings(const Xtring &filename, const Xtring &desc = Xtring::null)
        : mFilename( filename ), mDescription(desc), mIsReadOnly(false) {}
    virtual ~Settings() {}
    const Xtring &getName() const {
        return mFilename;
    }
    const Xtring &getDescription() const {
        return mDescription;
    }
    bool isReadOnly() const {
        return mIsReadOnly;
    }
    void setReadOnly(bool ro) {
        mIsReadOnly = ro;
    }
    virtual bool read(const Xtring &filename = Xtring::null);
    virtual bool write(const Xtring &filename = Xtring::null);
    bool readString(const char *buffer);
    void clear();
    Xtring toString() const;

    Variant getValue(const Xtring &settingname, const Variant &defaultvalue = Variant()) const;
    virtual void setValue(const Xtring &settingname, const Variant &val);
    virtual void removeSetting(const Xtring &settingname);
    const SettingsDict& allSettings() const {
        return mSettingsDict;
    }
protected:
    SettingsDict mSettingsDict;

private:
    Xtring getNextToken( const char **text );
    Xtring getNextValue( const char **text );
    Xtring mFilename, mDescription;
    bool mIsReadOnly;
    int mLine, mCol;
};

// Stream functions
#ifdef _GONG_DEBUG
std::ostream &operator<<(std::ostream &, const Settings &);
#endif

} // namespace gong
#endif // _GONG_SETTINGS_H
