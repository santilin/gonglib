#ifndef RTKINPUTFIELD_H
#define RTKINPUTFIELD_H

/*<<<<<COPYLEFT*/
/** @file rtkinputfield.h Input field object
 * RTK The report toolkit. Copyright (C) 2003-2013 Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
#include <gongxtring.h>
#include <gongvariant.h>
#include "rtkproperty.h"
#include "rtkparameter.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

class InputField
{
public:
    InputField(const char * name);
    virtual ~InputField() {}

    int sourcePos() const {
        return mSourcePos;
    }
    void setSourcePos(int pos) {
        mSourcePos = pos;
    }
    Variant value() const {
        return mValue;
    }
    void setValue(const Variant &value) {
        mValue = value;
    }

    /*<<<<<INPUTFIELD_PROPERTIES*/
public:
    const char * name() const {
        return propName.get();
    }
    const char * source() const {
        return propSource.get();
    }
    void setOrigSource(const char * source )		{
        propSource.setOrig(source);
    }
    Variant::Type type() const {
        return propType.get();
    }
    void setOrigType(const char * type )		{
        propType.setOrig(type);
    }
    const char * constValue() const {
        return propConstValue.get();
    }
    void setOrigConstValue(const char * constvalue )		{
        propConstValue.setOrig(constvalue);
    }
    virtual void fixParameters(const ParametersList &parameters, const char *delim); // from Input

public:
    ReadOnlyProperty<const char *> propName;
    FixableProperty<const char *> propSource;
    FixableProperty<Variant::Type> propType;
    FixableProperty<const char *> propConstValue;
    /*>>>>>INPUTFIELD_PROPERTIES*/


private:
    void init();
    int mSourcePos;
    Variant mValue;
};

} // namespace

#endif
