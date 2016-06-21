/*<<<<<DBFIELDDEFINITION_PREAMBLE*/
#ifndef ___GONGDBFIELDDEFINITION_H
#define ___GONGDBFIELDDEFINITION_H
/*>>>>>DBFIELDDEFINITION_PREAMBLE*/

/** @file gongdbfielddefinition.h Table and view field definitions
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
#include "gongdictionary.h"
#include "gongdbfieldvalue.h"
#include "gongdbvalidresult.h"

namespace gong {

class dbRecord;
class dbTableDefinition;

class dbFieldDefinition
{
public:
    enum Flags {
        NONE = 0x0000,
        NOTNULL = 0x0001,
        PRIMARYKEY = 0x0002,
        SEQUENCE = 0x0004,
        UNIQUE = 0x0008,
        REFERENCE = 0x0010,
        CODE = 0x0020,
        DESCRIPTION = 0x0040,
        ASCENDENT = 0x0080,
        DESCENDENT = 0x0100,
        INVISIBLE = 0x0200,
        READONLY = 0x0400,
        CALCULATED = 0x0800,
        PASSWORD = 0x1000,
		SECONDARYCODE = 0x2000
    };

    inline friend Flags operator|=( Flags &lhs, Flags rhs ) {
        return lhs = Flags( long( lhs ) | long( rhs ) );
    }
    inline friend Flags operator| ( Flags &lhs, Flags rhs ) {
        return Flags( long( lhs ) | long( rhs ) );
    }
    inline friend Flags operator&=( Flags &lhs, Flags rhs ) {
        return lhs = Flags( long( lhs ) & long( rhs ) );
    }
    inline friend Flags operator~( Flags lhs ) {
        return dbFieldDefinition::Flags( ~long( lhs ) ) ;
    }

    dbFieldDefinition( const Xtring& tablename, const Xtring& name, const SqlColumnType& sqlcolumntype,
                       const unsigned& sqlwidth, const unsigned& decimals = unsigned(),
                       const dbFieldDefinition::Flags& flags = dbFieldDefinition::NONE,
                       const Xtring& defaultvalue = Xtring::null)
        : mTableName( tablename ), mName( name ), mSqlColumnType( sqlcolumntype ),
          mSqlWidth( sqlwidth ), mDecimals( decimals ), mFlags( flags ), mDefaultValue( defaultvalue ),
          mDisplayWidth(0)
    {
        init_class();
    }

    // Copy constructor
    dbFieldDefinition( const dbFieldDefinition &other );

public:
    const Xtring& getTableName() const {
        return mTableName;
    }
    const Xtring& getName() const {
        return mName;
    }
    const SqlColumnType& getSqlColumnType() const {
        return mSqlColumnType;
    }
    const unsigned& getSqlWidth() const {
        return mSqlWidth;
    }
    const unsigned& getDecimals() const {
        return mDecimals;
    }
    const Xtring& getCaption() const {
        return mCaption;
    }
    const dbFieldDefinition::Flags& getFlags() const {
        return mFlags;
    }
    const Xtring& getDescription() const {
        return mDescription;
    }
    const Xtring& getDefaultValue() const {
        return mDefaultValue;
    }
    const Xtring& getReference() const {
        return mReference;
    }
    const Xtring& getStyle() const {
        return mStyle;
    }
    const unsigned& getDisplayWidth() const {
        return mDisplayWidth;
    }

    dbFieldDefinition* setSqlColumnType( const SqlColumnType& sqlcolumntype ) {
        mSqlColumnType = sqlcolumntype;
        return this;
    }
    const Xtring &getOrigDDL() const {
        return mOrigDDL;
    }

    dbFieldDefinition* setSqlWidth( const unsigned& sqlwidth );
    dbFieldDefinition* setDecimals( const unsigned& decimals ) {
        mDecimals = decimals;
        return this;
    }
    dbFieldDefinition* setCaption( const Xtring& caption ) {
        mCaption = caption;
        return this;
    }
    dbFieldDefinition* setFlags( const dbFieldDefinition::Flags& flags ) {
        mFlags = flags;
        return this;
    }
    dbFieldDefinition* setDescription( const Xtring& description ) {
        mDescription = description;
        return this;
    }
    dbFieldDefinition* setDefaultValue( const Xtring& defaultvalue ) {
        mDefaultValue = defaultvalue;
        return this;
    }
    dbFieldDefinition* setReference( const Xtring& reference ) {
        mReference = reference;
        return this;
    }
    dbFieldDefinition* setStyle( const Xtring& style ) {
        mStyle = style;
        return this;
    }
    dbFieldDefinition* setDisplayWidth( const unsigned& dispwidth ) {
        mDisplayWidth = dispwidth;
        return this;
    }
    dbFieldDefinition* setOrigDDL( const Xtring &origddl ) {
        mOrigDDL = origddl;
        return this;
    }

public:
    bool canBeNull() const {
        return !( mFlags & NOTNULL );
    }
    bool isUnique() const {
        return mFlags & UNIQUE;
    }
    bool isPrimaryKey() const {
        return mFlags & PRIMARYKEY;
    }
    bool isSequence() const {
        return mFlags & SEQUENCE;
    }
    bool isReference() const {
        return mFlags & REFERENCE;
    }
    bool isVisible() const {
        return !( mFlags & INVISIBLE );
    }
    bool isReadOnly() const {
        return mFlags & READONLY;
    }
    bool isCalculated() const {
        return mFlags & CALCULATED;
    }
    bool isCode() const {
        return mFlags & CODE;
    }
    bool isDescription() const {
        return mFlags & DESCRIPTION;
    }
    bool isAscendent() const {
        return mFlags & ASCENDENT;
    }
    bool isDescendent() const {
        return mFlags & DESCENDENT;
    }
    bool isSecondaryCode() const {
        return mFlags & SECONDARYCODE;
    }
    dbFieldDefinition* setCanBeNull( bool canbenull ) {
        if ( !canbenull ) mFlags |= NOTNULL;
        else mFlags &= ~NOTNULL;
        return this;
    }
    dbFieldDefinition* setUnique( bool unique ) {
        if ( unique ) mFlags |= UNIQUE;
        else mFlags &= ~UNIQUE;
        return this;
    }
    dbFieldDefinition* setPrimaryKey( bool primarykey ) {
        if ( primarykey ) mFlags |= PRIMARYKEY;
        else mFlags &= ~PRIMARYKEY;
        return this;
    }
    dbFieldDefinition* setSequence( bool sequence ) {
        if ( sequence ) mFlags |= SEQUENCE;
        else mFlags &= ~SEQUENCE;
        return this;
    }
    dbFieldDefinition* setIsReference( bool isreference ) {
        if ( isreference ) mFlags |= REFERENCE;
        else mFlags &= ~REFERENCE;
        return this;
    }
    dbFieldDefinition* setVisible( bool visible ) {
        if ( visible ) mFlags &= ~INVISIBLE;
        else mFlags |= INVISIBLE;
        return this;
    }
    dbFieldDefinition* setReadOnly( bool readonly ) {
        if ( readonly ) mFlags |= READONLY;
        else mFlags &= ~READONLY;
        return this;
    }
    dbFieldDefinition* setIsCode( bool iscode ) {
        if ( iscode ) mFlags |= CODE;
        else mFlags &= ~CODE;
        return this;
    }
    dbFieldDefinition* setIsDescription( bool isdescription ) {
        if ( isdescription ) mFlags |= DESCRIPTION;
        else mFlags &= ~DESCRIPTION;
        return this;
    }
    dbFieldDefinition* setCalculated( bool notimport = true ) {
        if ( notimport ) mFlags |= CALCULATED;
        else mFlags &= ~CALCULATED;
        return this;
    }
    dbFieldDefinition* setIsAscendent( bool isascendent ) {
        if ( isascendent ) mFlags |= ASCENDENT;
        else mFlags &= ~ASCENDENT;
        return this;
    }
    dbFieldDefinition* setIsDescendent( bool isdescendent ) {
        if ( isdescendent ) mFlags |= DESCENDENT;
        else mFlags &= ~DESCENDENT;
        return this;
    }
    dbFieldDefinition* setIsSecondaryCode( bool secondarycode ) {
        if ( secondarycode ) mFlags |= SECONDARYCODE;
        else mFlags &= ~SECONDARYCODE;
        return this;
    }
    int getStyleWidth(const class dbDefinition *pdb) const;

    virtual ~dbFieldDefinition() {};
    virtual Xtring getDisplayValue(const Variant &value) const;
    virtual bool isValid( dbRecord *r, dbFieldValue *value,
                          ValidResult::Context context, ValidResult *integres) const;
    virtual Xtring toSQL( dbConnection *conn, const dbFieldValue &value, bool inserting = false ) const;
    virtual Xtring toDDL( dbConnection *conn ) const;
    virtual Variant customDefaultValue() const;
    /** Redefine if your field definition adds some extra data */
    virtual dbFieldDefinition *clone() const;

    /** Gets the fullname (tablename.fieldname) of the field */
    const Xtring getFullName() const {
        return mTableName + "." + mName;
    };
    /** Gets the variant type equivalent of the sqlcolumntype */
    Variant::Type getVariantType() const;
    /** Compares two dbFieldDefinitions and returns a DDL statement to change one into the other */
    Xtring sameSQLSchema( const dbFieldDefinition *other, dbConnection *conn, bool purging = false ) const;
    /** Extracts the Table part of a full qualified field name */
    static Xtring extractTableName( const Xtring &fullfldname );
    /** Extracts the Field part of a full qualified field name */
    static Xtring extractFieldName( const Xtring &fullfldname );
    dbFieldValue createFieldValue() const;
    /** finds the relation definition for this field in its parent's definition */
    class dbRelationDefinition *findRelationDefinition( const dbTableDefinition &tlbdef ) const;
private:
    void init_class();

    Xtring mTableName;
    Xtring mName;
    SqlColumnType mSqlColumnType;
    unsigned mSqlWidth;
    unsigned mDecimals;
    Xtring mCaption;
    dbFieldDefinition::Flags mFlags;
    Xtring mDescription;
    Xtring mDefaultValue;
    Xtring mReference;
    Xtring mStyle;
    unsigned mDisplayWidth;
    Xtring mOrigDDL; // Only if created with fromSqlSchema
};


typedef Dictionary<dbFieldDefinition *> dbFieldDefinitionDict;

// Stream functions
#ifdef _GONG_DEBUG
std::ostream &operator<<( std::ostream &, const dbFieldDefinition & );
#endif


/*<<<<<DBFIELDDEFINITION_POSTAMBLE*/
} // namespace gong
#endif // __GONGDBFIELDDEFINITION_H
/*>>>>>DBFIELDDEFINITION_POSTAMBLE*/

