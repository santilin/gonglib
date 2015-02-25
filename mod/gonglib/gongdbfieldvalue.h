#ifndef _GONG_DBFIELDVALUE_H
#define _GONG_DBFIELDVALUE_H

/** @file gongdbfieldvalue.h Field value templates
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

namespace gong {

class dbFieldDefinition;

class dbFieldValue
{
public:
    dbFieldValue(const dbFieldDefinition *flddef, const Variant &value = Variant(), bool isnull = true )
        : mValue(value), mNull(isnull), mModified(false) {}
    dbFieldValue(const dbFieldDefinition *flddef, Variant::Type type, bool isnull = true );
    ~dbFieldValue() {
        clear();
    }

    Xtring toString() const {
        return mValue.toString();
    }
    void clear( const Variant &defvalue = Variant());

    Variant value() const { return mValue; }
    void setValue(const Variant &value);
    void setNull() {
        if( !isNull() ) {
            clear();
            mModified = true;
        }
    }

    bool operator==(const Variant &other) {
        return (mValue == other);
    }
    bool operator!=(const Variant &other) {
        return !operator==(other);
    }
    bool operator<=(const Variant &other) {
        return (mValue <= other);
    }
    bool operator<(const Variant &other) {
        return (mValue < other);
    }
    bool isEmpty() const;
    bool isNull() const {
        return mNull;
    }
    bool isModified() const {
        return mModified;
    }
    void setModified(bool modified=true) {
        mModified = modified;
    }
protected:
    Variant mValue;
    bool mNull;
    bool mModified;
};

}; // namespace gong

#endif // _GONG_DBFIELDVALUE_H
