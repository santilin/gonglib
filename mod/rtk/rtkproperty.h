#ifndef RTKPROPERTY_H
#define RTKPROPERTY_H

/*<<<<<COPYLEFT*/
/** @file rtkproperty.h Property templates
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
#include "rtknamespace.h"

namespace gong {}
namespace RTK = gong;

namespace gong {

/**
@author Francisco Santiago Capel Torres
*/

extern const char *const null_char_ptr;

template<class ValueT>
class ReadOnlyProperty
{
public:
    ReadOnlyProperty(const ValueT &value = ValueT() ): mValue(value) {}
    ~ReadOnlyProperty() {
        clear();
    }
    ValueT get() const {
        return mValue;
    }
    bool operator==(const ValueT &other) {
        return (mValue == other);
    }
    bool operator!=(const ValueT &other) {
        return !operator==(other);
    }
    bool operator<=(const ValueT &other) {
        return (mValue <= other);
    }
    bool operator<(const ValueT &other) {
        return (mValue < other);
    }
    bool isEmpty() const {
        return !mValue;
    }
protected:
    void clear() {
        mValue = ValueT();
    }
    ValueT mValue;
};


template<> inline
ReadOnlyProperty<const char *>::ReadOnlyProperty(const char *const &value)
    : mValue(value?strdup(value):null_char_ptr)
{}

template<> inline
void ReadOnlyProperty<const char *>::clear()
{
    if( mValue && mValue != null_char_ptr ) delete mValue;
    mValue = null_char_ptr;
}

template<> inline
bool ReadOnlyProperty<const char *>::isEmpty() const
{
    return strempty(mValue);
}


template<> inline
bool ReadOnlyProperty<const char *>::operator==(const char *const &other)
{
    return !strcmp(mValue, other);
}


template<class ValueT>
class Property: public ReadOnlyProperty<ValueT>
{
public:
    Property(const ValueT &value = ValueT() ): ReadOnlyProperty<ValueT>(value), mIsNull(true) {}
    ~Property() {
        this->clear();
    }
    void set(const ValueT &newvalue) {
        this->clear();
        this->mValue = newvalue;
        mIsNull=false;
    }
    Property<ValueT> &operator=(const ValueT &newvalue) {
        set(newvalue);
        return *this;
    }
    bool isNull() const {
        return mIsNull;
    }
    void setDefault(const ValueT &newvalue) {
        this->mValue = newvalue;
        mIsNull=true;
    }
    void clear() { ReadOnlyProperty<ValueT>::clear(); mIsNull = true; }
protected:
    bool mIsNull;
};


template<> inline
void Property<const char *>::set(const char *const &other)
{
    clear();
    mValue = (!strempty(other)?strdup(other):null_char_ptr);
    mIsNull = false;
}

class Parameter;
typedef std::vector<Parameter *> ParametersList;
Xtring replaceParams(const char *orig,
                     const ParametersList &parameters, const Xtring &delim);

template<class ValueT>
class FixableProperty: public Property<ValueT>
{
public:
    FixableProperty(const ValueT &value = ValueT() ): Property<ValueT>(value), pOrigValue(0) {}
    ~FixableProperty() {
        this->clear();
        if( pOrigValue && pOrigValue != null_char_ptr) delete pOrigValue;
    }
    const char *getOrig() const {
        return pOrigValue;
    }
    FixableProperty<ValueT> &operator=(const ValueT &newvalue) {
        this->set(newvalue);
        return *this;
    }
    void setOrig(const char *origvalue)
    {
        if( pOrigValue && pOrigValue != null_char_ptr )
            delete pOrigValue;
        pOrigValue = (strempty(origvalue)?null_char_ptr:strdup(origvalue));
    }
    bool fix(const ParametersList &parameters, const Xtring &delim, const ValueT &defaultvalue = ValueT())
    {
		bool isvalid = true;
        ValueT tmpvalue = stringTo<ValueT>(
                              replaceParams(pOrigValue, parameters, delim).c_str(), &isvalid, &this->mIsNull);
        if( !this->mIsNull )
            this->set( tmpvalue );
        else {
            this->set( defaultvalue );
            this->mIsNull = true;
        }
        return isvalid;
    }
private:
    const char *pOrigValue;
};

template<> inline
bool FixableProperty<const char *>::fix(const ParametersList &parameters, const Xtring &delim,
        const char *const &defaultvalue)
{
    Xtring tmpvalue = replaceParams(pOrigValue, parameters, delim);
    if( tmpvalue.size() )
        set( tmpvalue.c_str() );
    else {
        set( defaultvalue );
        mIsNull = true;
    }
    return true;
}

} // namespace RTK

#endif
