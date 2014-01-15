#ifndef _GONG_DBMASTERTABLE_H
#define _GONG_DBMASTERTABLE_H

/** @file gongdbmastertable.h A table where fields can be easily added
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongdbtabledefinition.h"
#include "gongdbfielddefinition.h"
#include "gongdbfieldemail.h"
#include "gongdbfieldimage.h"

namespace gong {

class dbMasterTable
{
public:
    dbMasterTable( dbTableDefinition *tbldef ) {
        pTableDefinition = tbldef;
    }
    dbMasterTable( dbDefinition &db, const Xtring &name, const Xtring &from = Xtring::null,
                   const Xtring &where = Xtring::null, const Xtring &orderby = Xtring::null )
        : pTableDefinition( new dbTableDefinition( db, name, from, where, orderby ) ) {}

    dbTableDefinition *getTableDefinition() const {
        return pTableDefinition;
    }
    const Xtring &getDescSingular() const {
        return pTableDefinition->getDescSingular();
    }
    const Xtring &getDescPlural() const {
        return pTableDefinition->getDescPlural();
    }

    // A general field
    dbFieldDefinition *addField( const dbFieldDefinition *fielddef ) {
        return pTableDefinition->addField( fielddef );
    }
    /// Adds a dbRecordBehavior to this table definition
    bool addBehavior( dbRecordBehavior *behavior ) {
        return pTableDefinition->addBehavior( behavior );
    }

    // Basic fields
    dbFieldDefinition *addFieldString( const Xtring &name, unsigned int width,
                                       dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                       const Xtring &defaultvalue = Xtring::null ) {
        return pTableDefinition->addFieldString( name, width, flags, defaultvalue );
    }
    dbFieldDefinition *addFieldRecordID( const Xtring &idname = Xtring::null ) {
        return pTableDefinition->addFieldRecordID( idname );
    }
    dbFieldDefinition *addFieldBool( const Xtring &name,
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldBool( name, flags );
    }
    dbFieldDefinition *addFieldInt( const Xtring &name,
                                    dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldInt( name, flags );
    }
    dbFieldDefinition *addFieldFloat( const Xtring &name,
                                      dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldFloat( name, flags | dbFieldDefinition::NOTNULL );
    }
    dbFieldDefinition *addFieldDecimal( const Xtring &name, int width = 12, int decimals = 2,
                                        dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldDecimal( name, width, decimals, flags | dbFieldDefinition::NOTNULL );
    }
    dbFieldDefinition *addFieldMoney( const Xtring &name, int width = 12, int decimals = 2,
                                      dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldMoney( name, width, decimals, flags | dbFieldDefinition::NOTNULL );
    }
    dbFieldDefinition *addFieldDate( const Xtring &name,
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldDate( name, flags );
    }
    dbFieldDefinition *addFieldTime( const Xtring &name,
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldTime( name, flags );
    }
    dbFieldDefinition *addFieldDateTime( const Xtring &name,
                                         dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldDateTime( name, flags );
    }
    dbFieldDefinition *addFieldTimestamp( const Xtring &name,
                                          dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldTimestamp( name, flags );
    }
    dbFieldDefinition *addFieldText( const Xtring &name,
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldText( name, flags );
    }
    dbFieldDefinition *addFieldBinary( const Xtring &name,
                                       dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldBinary( name, flags );
    }
    dbFieldDefinition *addFieldReferenceID( const Xtring &name, const Xtring &referredField,
                                            dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addFieldReferenceID( name, referredField, flags );
    }
    dbFieldDefinition *addFieldOne2OneRelation( const Xtring &name,
            const Xtring &referredField, bool canbenull = false );
    dbFieldDefinition *addFieldOne2ManyRelation( const Xtring &name,
            const Xtring &referredField, bool canbenull = false );
    dbFieldDefinition *addFieldAggregateRelation( const Xtring &name,
            const Xtring &referredField, bool canbenull = true );
    template<class T> dbFieldDefinition *addFieldListOfValues( bool insertallowed,
            XtringList *captions, List<T> *values, const Xtring &name );
    template<class T> dbFieldDefinition *addFieldListOfValues( bool insertallowed,
            XtringList &captions, List<T> &values, const Xtring &name );

    // Fields with special meanings
    dbFieldDefinition *addFieldChar( const Xtring &name,
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return addFieldString( name, 1, flags );
    }
    dbFieldDefinition *addFieldPercent( const Xtring &name,
                                        dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addField( name, SQLFLOAT, 5, 2, flags | dbFieldDefinition::NOTNULL )
               ->setStyle( "PERCENT" );
    }
    dbFieldDefinition *addFieldIntCode( const Xtring &name,
                                        dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addField( name, SQLINTEGER, 0, 0,
                                           flags | dbFieldDefinition::Flags( dbFieldDefinition::UNIQUE | dbFieldDefinition::CODE
                                                   | dbFieldDefinition::NOTNULL ) )->setStyle( "CODE" );
    }
    dbFieldDefinition *addFieldDesc( const Xtring &name, int width = 0,
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addField( name, SQLSTRING, width, 0,
                                           flags | dbFieldDefinition::Flags( dbFieldDefinition::DESCRIPTION | dbFieldDefinition::NOTNULL ) );
    }
    dbFieldDefinition *addFieldStringCode( const Xtring &name, int width = 0,
                                           dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return pTableDefinition->addField( name, SQLSTRING, width, 0,
                                           flags | dbFieldDefinition::Flags( dbFieldDefinition::UNIQUE | dbFieldDefinition::CODE
                                                   | dbFieldDefinition::NOTNULL ) );
    }
    dbFieldDefinition *addFieldPassword( const Xtring &name = "PASSWORD",
                                         dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return addFieldString( name, 32, flags | dbFieldDefinition::PASSWORD );
    }
    dbFieldDefinition *addFieldFilename( const Xtring &name = "FILENAME",
                                         dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return addFieldString( name, 254, flags );
    }

    dbRelationDefinition *addRelation( const dbRelationDefinition::Type& type,
                                       const Xtring &lefttable, const Xtring &leftfield,
                                       const Xtring &righttable, const Xtring &rightfield ) {
        return pTableDefinition->addRelation( type, lefttable, leftfield, righttable, rightfield );
    }
    dbIndexDefinition *addIndex( const dbIndexDefinition *idxdef ) {
        return pTableDefinition->addIndex( idxdef );
    }
    dbIndexDefinition *addIndex( const dbFieldDefinition *flddef ) {
        return pTableDefinition->addIndex( flddef );
    }
    dbIndexDefinition *addMultipleIndex( const char *name, const Xtring &fields, bool unique = false ) {
        return pTableDefinition->addMultipleIndex(name, fields, unique);
    }

    // These allow you to add any kind of dbFieldDefinition
    template <class T> T* addField( const Xtring &name,
                                    dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                    const Xtring &defaultvalue = Xtring::null ) {
        return static_cast<T *>
               ( pTableDefinition->addField( new T( pTableDefinition->getName(), name, flags, defaultvalue ) ) );
    }
    template <class T> T* addField( const Variant &extraarg, const Xtring &name,
                                    dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                    const Xtring &defaultvalue = Xtring::null ) {
        return static_cast<T *>
               ( pTableDefinition->addField( new T( extraarg, pTableDefinition->getName(), name, flags, defaultvalue ) ) );
    }

protected:
    dbTableDefinition *pTableDefinition;
};


}; // namespace gong
#endif // _GONG_DBMASTERTABLE_H

