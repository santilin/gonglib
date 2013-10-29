#ifndef _GONGDB_TABLEDEFINITION_H
#define _GONGDB_TABLEDEFINITION_H

/** @file gongdbtabledefinition.h database table definition
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include "gongdictionary.h"
#include "gongdbrelationdefinition.h"
#include "gongdbindexdefinition.h"
#include "gongdbfieldreferenceid.h"
#include "gongdbrecordbehavior.h"

namespace gong {

class dbConnection;
class dbDefinition;

class dbTableDefinition
{
public:
    dbTableDefinition( dbDefinition &db, const Xtring& name,
                       const Xtring& from = Xtring::null, const Xtring& where = Xtring::null,
                       const Xtring& orderby = Xtring::null, const Xtring& groupby = Xtring::null );
    // Copy constructor
    dbTableDefinition( const dbTableDefinition &other );

    dbDefinition &getdbDefinition() const {
        return mdbDefinition;
    }
    /** Creates a dbTableDefinition from the schema of a SQL database */
    static dbTableDefinition *fromSQLSchema( dbConnection *conn, dbDefinition &db,
            const Xtring &tablename );
    static bool parseSelectStatement( const Xtring &select, Xtring &fields, Xtring &from,
                                      Xtring &where, Xtring &orderby, Xtring &groupby );
    ~dbTableDefinition();

    Xtring getFullFldName( const Xtring &fldname ) const;
    dbFieldDefinition *findFieldDefinition( const Xtring &name, bool warning = false ) const;
    dbFieldDefinition *getFieldDefinition( unsigned int i ) const;
    unsigned int getFieldCount() const {
        return mFieldDefinitions.size();
    }
    int getFieldPosition( const Xtring &fldname ) const;
    void clearFields();

    dbFieldDefinition *findFieldByFlags( const dbFieldDefinition::Flags &flags ) const;

    dbIndexDefinition *getIndexDefinition( unsigned int i ) {
        return mIndexDefinitions[i];
    }
    const dbIndexDefinition *getIndexDefinition( unsigned int i ) const {
        return mIndexDefinitions[i];
    }
    void clearIndexes();

    /** Adds a general field */
    dbFieldDefinition *addField( const dbFieldDefinition *fielddef );
    dbFieldDefinition *addField( const Xtring &name, SqlColumnType sqltype,
                                 unsigned int width, unsigned short int decimals,
                                 dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                 const Xtring &defaultvalue = Xtring::null );
    bool deleteField( const dbFieldDefinition *fielddef );
    bool deleteField( const Xtring &name );

    dbRelationDefinition *addRelation( const dbRelationDefinition::Type& type,
                                       const Xtring &lefttable, const Xtring &leftfield,
                                       const Xtring &righttable, const Xtring &rightfield );
    dbIndexDefinition *addIndex( const dbIndexDefinition *idxdef );
    dbIndexDefinition *addIndex( const dbFieldDefinition *flddef );
    dbIndexDefinition *addMultipleIndex( const char *name, const Xtring &fields, bool unique = false );
    dbIndexDefinition *deleteIndex( const char *name );

    /// Adds a dbRecordBehavior to this table definition
    bool addBehavior( dbRecordBehavior *behavior );
    const dbRecordBehaviorsList &getRecordBehaviors() const {
        return mRecordBehaviors;
    }

    // Operations
    bool create( dbConnection *conn, bool ifnotexists = false,
                 const Xtring &extraargs = Xtring::null, bool ignoreerrors = false );
    bool createIndexes( dbConnection *conn, bool ignoreerrors = false );
    bool dropIndexes( dbConnection *conn, bool removeall, bool ignoreerrors );
    Xtring sameSQLSchema( const dbTableDefinition *other, dbConnection *conn, bool purging = false ) const;

    // Funtions for easy addition of fields
    dbFieldDefinition *addFieldString( const Xtring &name, unsigned int width,
                                       dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                       const Xtring &defvalue = Xtring::null ) {
        return addField( name, SQLSTRING, width, 0,
                         flags, defvalue );
    }
    dbFieldDefinition *addFieldRecordID( const Xtring &idname = Xtring::null );
    dbFieldDefinition *addFieldBool( const Xtring &name,
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE, const Xtring &defvalue = "0" ) {
        return addField( name, SQLBOOL, 1, 0,
                         flags | dbFieldDefinition::NOTNULL, defvalue)->setStyle("BOOL" );
    }
    dbFieldDefinition *addFieldInt( const Xtring &name,
                                    dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                    const Xtring &defvalue = "0" ) {
        return addField( name, SQLINTEGER, 0, 0,
                         flags | dbFieldDefinition::NOTNULL, defvalue )->setStyle( "INTEGER" );
    }
    dbFieldDefinition *addFieldFloat( const Xtring &name,
                                      dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                      const Xtring &defvalue = "0.0" ) {
        return addField( name, SQLFLOAT, 0, 0, flags | dbFieldDefinition::NOTNULL,
                         defvalue )->setStyle( "DECIMAL" );
    }
    dbFieldDefinition *addFieldDecimal( const Xtring &name, int width = 12, int decimals = 2,
                                        dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                        const Xtring &defvalue = "0.0" ) {
        return addField( name, SQLDECIMAL, width, decimals, flags | dbFieldDefinition::NOTNULL,
                         defvalue )->setStyle( "DECIMAL" );
    }
    dbFieldDefinition *addFieldMoney( const Xtring &name, int width = 12, int decimals = 2,
                                      dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                      const Xtring &defvalue = Xtring::null ) {
        return addField( name, SQLDECIMAL, width, decimals,
                         flags | dbFieldDefinition::NOTNULL, defvalue )->setStyle( "MONEY" );
    }
    dbFieldDefinition *addFieldDate( const Xtring &name,
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                     const Xtring &defvalue = Xtring::null ) {
        return addField( name, SQLDATE, 0, 0, flags, defvalue )->setStyle( "DATE" );
    }
    dbFieldDefinition *addFieldTime( const Xtring &name,
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                     const Xtring &defvalue = Xtring::null ) {
        return addField( name, SQLTIME, 0, 0, flags, defvalue )->setStyle( "TIME" );
    }
    dbFieldDefinition *addFieldDateTime( const Xtring &name,
                                         dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                         const Xtring &defvalue = Xtring::null ) {
        return addField( name, SQLDATETIME, 0, 0, flags, defvalue)->setStyle("DATETIME" );
    }
    dbFieldDefinition *addFieldTimestamp( const Xtring &name,
                                          dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                          const Xtring &defvalue = Xtring::null ) {
        return addField( name, SQLTIMESTAMP, 0, 0, flags, defvalue)->setStyle("DATETIME" );
    }
    dbFieldDefinition *addFieldText( const Xtring &name,
                                     dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                     const Xtring &defvalue = Xtring::null ) {
        return addField( name, SQLTEXT, 0, 0, flags, defvalue );
    }
    dbFieldDefinition *addFieldBinary( const Xtring &name,
                                       dbFieldDefinition::Flags flags = dbFieldDefinition::NONE,
                                       const Xtring &defvalue = Xtring::null ) {
        return addField( name, SQLBLOB, 0, 0, flags, defvalue );
    }
    dbFieldReferenceID *addFieldReferenceID( const Xtring &name, const Xtring &referredField,
            dbFieldDefinition::Flags flags = dbFieldDefinition::NONE ) {
        return static_cast<dbFieldReferenceID *>( addField( new dbFieldReferenceID( getName(), name, referredField,
                flags | dbFieldDefinition::INVISIBLE ) ) );
    }

    const Xtring& getName() const {
        return mName;
    }
    void setName( const Xtring &name ) {
        mName = name;
    }
    const Xtring& getDescPlural() const {
        return mDescPlural;
    }
    const Xtring& getDescSingular() const {
        return mDescSingular;
    }
    bool isFemenina() const {
        return mFemenina;
    }
    const Xtring& getFrom() const {
        return mFrom;
    }
    const Xtring& getWhere() const {
        return mWhere;
    }
    const Xtring& getOrderBy() const {
        return mOrderBy;
    }
    const Xtring& getGroupBy() const {
        return mGroupBy;
    }
    const dbFieldDefinitionsList& getFieldDefinitions() const {
        return mFieldDefinitions;
    }
    const dbRelationDefinitionsList& getRelationDefinitions() const {
        return mRelationDefinitions;
    }
    const dbIndexDefinitionsList& getIndexDefinitions() const {
        return mIndexDefinitions;
    }
    const Xtring& getFldIDName() const;
    bool isTemporary() const {
        return mTemporary;
    }

    dbTableDefinition* setDescPlural( const Xtring& descplural ) {
        mDescPlural = descplural;
        return this;
    }
    dbTableDefinition* setDescSingular( const Xtring& descsingular ) {
        mDescSingular = descsingular;
        return this;
    }
    dbTableDefinition* setFemenina( bool femenina ) {
        mFemenina = femenina;
        return this;
    }
    dbTableDefinition* setFrom( const Xtring& from ) {
        mFrom = from;
        return this;
    }
    dbTableDefinition* setWhere( const Xtring& where ) {
        mWhere = where;
        return this;
    }
    dbTableDefinition* setOrderBy( const Xtring& orderby ) {
        mOrderBy = orderby;
        return this;
    }
    dbTableDefinition* setGroupBy( const Xtring& groupby ) {
        mGroupBy = groupby;
        return this;
    }
    void setTemporary( bool temporary = true ) {
        mTemporary = temporary;
    }

private:
    dbDefinition &mdbDefinition;
    Xtring mName;
    Xtring mDescPlural;
    Xtring mDescSingular;
    bool mFemenina, mTemporary;
    Xtring mFrom;
    Xtring mWhere;
    Xtring mOrderBy;
    Xtring mGroupBy;
    dbFieldDefinitionsList mFieldDefinitions;
    dbRelationDefinitionsList mRelationDefinitions;
    dbIndexDefinitionsList mIndexDefinitions;
    dbRecordBehaviorsList mRecordBehaviors;

    Xtring mFldIDName;
};

typedef Dictionary<dbTableDefinition *> dbTableDefinitionsList;

// Stream functions
#ifdef _GONG_DEBUG
std::ostream &operator<<( std::ostream &, const dbTableDefinition & );
#endif

} // namespace gong
#endif // _GONG_DBTABLEDEFINITION_H
