#ifndef _GONG_GONGDBFIELDLISTOFVALUES_HPP
#define _GONG_GONGDBFIELDLISTOFVALUES_HPP

/** @file gongdbfieldlistofvalues.hpp A dbFieldDefinition for lists of values
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
#include "gongdbfieldlistofvalues.h"

namespace gong {

template<class T>
void dbFieldListOfValues<T>::addValue(const Xtring &newcaption, const T &newvalue)
{
    if( mRefListOfCaptions.contains( newcaption ) )
        return;
    const_cast<XtringList &>(mRefListOfCaptions).push_back( newcaption );
	const_cast<List<T> &>(mRefListOfValues).push_back( newvalue );
}

template<class T>
const Xtring &dbFieldListOfValues<T>::findText(T value) const
{
    for( uint i=0; i<mRefListOfValues.size(); i++ )
        if( mRefListOfValues.at(i) == value )
            return mRefListOfCaptions.at(i);
    return Xtring::null;
}

template<class T>
const T dbFieldListOfValues<T>::findValue(const Xtring &caption) const
{
    for( uint i=0; i<mRefListOfCaptions.size(); i++ )
        if( mRefListOfCaptions.at(i).upper() == caption.upper() )
            return mRefListOfValues.at(i);
    return T();
}

// Template instantiation

template<>
bool dbFieldListOfValues<int>::isValid( dbRecord *r, dbFieldValue *value,
                                        ValidResult::Context context, ValidResult *integres ) const
{
    if( value->isEmpty() && (canBeNull() || mRefListOfValues.size() == 0) )
        return true;
    if( value->isEmpty() && !canBeNull() && mRefListOfValues.size() > 0 ) {
        if( integres )
            integres->addError( "'" + getCaption() + "' no puede quedarse vacío", getName() );
        return false;
    }
    if( !dbFieldDefinition::isValid(r, value, context, integres ) )
        return false;
    if( value->toVariant().type() == Variant::tInt
            && mRefListOfValues.contains( value->toVariant().toInt() ) ) {
        return true;
    } else if( mRefListOfCaptions.contains( value->toString() ) && context == ValidResult::fixing ) {
        // Fix value if the field has the caption instead of the value
        value->setValue( findValue( value->toString() ) );
        return true;
    } else if( !value->isEmpty() && !mInsertAllowed ) {
        if( integres )
            integres->addError( "'" + getCaption() + "' no acepta el valor '" + value->toString() + "'", getName() );
        return false;
    }
    return true;
}

template<>
bool dbFieldListOfValues<Xtring>::isValid( dbRecord *r, dbFieldValue *value,
        ValidResult::Context context, ValidResult *integres ) const
{
    if( value->isEmpty() && (canBeNull() || mRefListOfValues.size() == 0) )
        return true;
    if( !dbFieldDefinition::isValid(r, value, context, integres ) )
        return false;
    if( value->toVariant().type() == Variant::tString
            && mRefListOfValues.contains( value->toString() ) ) {
        return true;
    } else if( mRefListOfCaptions.contains( value->toString() ) && context == ValidResult::fixing ) {
        // Fix value if the field has the caption instead of the value
        value->setValue( findValue( value->toString() ) );
        return true;
    } else if( !value->isEmpty() && !mInsertAllowed ) {
        if( integres )
            integres->addError( "'" + getCaption() + "' no acepta el valor '" + value->toString() + "'", getName() );
        return false;
    }
    return true;
}

template<>
void dbFieldListOfValues<int>::setValuesFromString(const Xtring &values)
{
    const_cast<IntList &>(mRefListOfValues).clear();
    const_cast<XtringList &>(mRefListOfCaptions).clear();
    XtringList valuesandcaptions;
    values.tokenize( valuesandcaptions, "|" );
    uint nitem = 0;
    for( XtringList::const_iterator it = valuesandcaptions.begin();
            it != valuesandcaptions.end();
            ++it, ++nitem ) {
        Xtring value, caption;
        (*it).splitIn2( value, caption, "=" );
        if( caption.isEmpty() ) {// Only caption, so values are numbers
            addValue( value, nitem );
        } else {
            addValue( caption, value.toInt() );
        }
	}
}

template<>
void dbFieldListOfValues<Xtring>::setValuesFromString(const Xtring &values)
{
    mListOfValues.clear();
    mListOfCaptions.clear();
    XtringList valuesandcaptions;
    values.tokenize( valuesandcaptions, "|" );
    for( XtringList::const_iterator it = valuesandcaptions.begin();
            it != valuesandcaptions.end();
            ++it ) {
        Xtring value, caption;
        (*it).splitIn2( value, caption, "=" );
        if( caption.isEmpty() ) // Only caption, so values = captions
            caption = value;
        addValue( caption, value );
    }
}

template<>
Xtring dbFieldListOfValues<int>::getDisplayValue( const Variant& value ) const
{
    return findText( value.toInt() );
}

template<>
Xtring dbFieldListOfValues<Xtring>::getDisplayValue( const Variant& value ) const
{
    return findText( value.toString() );
}


}; // namespace gong
#endif // GONG_GONGDBFIELDLISTOFVALUES_H

