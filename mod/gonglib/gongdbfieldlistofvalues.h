/*<<<<<LISTOFVALUES_PREAMBLE*/
#ifndef _GONG_GONGDBFIELDLISTOFVALUES_H
#define _GONG_GONGDBFIELDLISTOFVALUES_H
/*>>>>>LISTOFVALUES_PREAMBLE*/

/** @file gongdbfieldlistofvalues.h A dbFieldDefinition for lists of values
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gonglist.h>
/*<<<<<DBFIELD_LISTOFVALUES_INCLUDES*/
#include <gongdbfielddefinition.h>
/*>>>>>DBFIELD_LISTOFVALUES_INCLUDES*/

namespace gong {

template<class T>
/**
 * @brief ...
 **/
class dbFieldListOfValues: public dbFieldDefinition
{
public:
    typedef List<T> ValueListType;
    dbFieldListOfValues(bool insertallowed,
                        XtringList *captions, ValueListType *values, const Xtring& tablename, const Xtring& name,
                        SqlColumnType sqltype, unsigned short int width, unsigned short int decimals = 0,
                        dbFieldDefinition::Flags flags = NONE, const Xtring& defaultvalue = Xtring::null)
        : dbFieldDefinition(tablename, name, sqltype, width, decimals, flags, defaultvalue),
          mInsertAllowed(insertallowed), pListOfCaptions(captions), pListOfValues(values)
    {}

    virtual dbFieldListOfValues *clone() const {
        return new dbFieldListOfValues( *this );
    }
    ValueListType *getListOfValues() const {
        return pListOfValues;
    }
    XtringList *getListOfCaptions() const {
        return pListOfCaptions;
    }
    virtual bool isValid( dbRecord *r, dbFieldValue *value,
                          ValidResult::Context context, ValidResult *integres) const; // from dbFieldDefinition
    virtual Xtring getDisplayValue(const Variant &value) const; // From dbFieldDefinition
    void addValue(const Xtring &newcaption, const T &newvalue);
    const Xtring &findText( T value ) const;
    const T findValue( const Xtring &caption ) const;
    void setValuesFromString(const Xtring &key);
    dbFieldDefinition* setInsertAllowed( bool ia = true ) {
        mInsertAllowed = ia;
        return this;
    }
protected:
    bool mInsertAllowed;
    /**
     * @brief Must be references as they are shared by all the field definitions in the database
     **/
    XtringList *pListOfCaptions;
    ValueListType *pListOfValues;
};

template<> inline
dbFieldListOfValues<int>::dbFieldListOfValues(bool insertallowed,
        XtringList *captions, List<int> *values, const Xtring& tablename, const Xtring& name,
        SqlColumnType sqltype, unsigned short int width, unsigned short int decimals,
        dbFieldDefinition::Flags flags, const Xtring& defaultvalue)
    : dbFieldDefinition(tablename, name, sqltype, width, decimals, flags, defaultvalue),
      mInsertAllowed(insertallowed), pListOfCaptions(captions), pListOfValues(values)
{
    // Add int values if none of them are specified
    if( values && captions ) {
        for( uint i = values->size(); i < captions->size(); ++ i ) {
            values->push_back( i + 1 );
        }
    }
}

}; // namespace gong
#endif // GONG_GONGDBFIELDLISTOFVALUES_H

