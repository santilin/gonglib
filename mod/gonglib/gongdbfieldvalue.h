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

class dbFieldValue: public Variant
{
public:
	dbFieldValue() {};
	dbFieldValue(const Variant &v);
    dbFieldValue(Variant::Type type, bool isnull, const dbFieldDefinition *flddef = 0);
	~dbFieldValue() {}
    bool isEmpty() const;
	dbFieldValue &operator=(const Variant &other);
	bool operator==(const Variant &other) const;


	dbFieldValue &setValue(const Variant &value);
    void setNull() {
        if( !isNull() ) {
            clear();
            mModified = true;
        }
    }
    bool isNull() const {
        return mNull;
    }
    bool isModified() const {
        return mModified;
    }
    void setModified(bool modified=true) {
        mModified = modified;
    }
    double toDouble(bool * ok = 0) const;
	void clear( const Variant &defvalue = Variant() );

protected:
    bool mNull;
    bool mModified;
};

}; // namespace gong

#endif // _GONG_DBFIELDVALUE_H
