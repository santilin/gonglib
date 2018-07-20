#ifdef _GONG_DEBUG
#include <iostream>
#endif
#include "gonggettext.h"
#include "gongdbdefinition.h"
#include "gongdbfielddefinition.h"
#include "gongdbfieldstyle.h"

/**
	@class gong::dbFieldDefinition
	@brief Table and view field definitions

	This class is part of the data dictionary definition and holds the properties of the fields for
	dbTableDefinitions, dbIndexDefinitions and dbViewDefinitions.

	There is only one constructor that takes all the properties as arguments.
	The easiest way for creating field definitions, then, is to add them to dbTableDefinition objects
	via their multiple addFieldXXXXXX functions.

	Note that the actual value of the field is not held in this definition. dbRecord is used to create
	the fields from dbTableDefinitions and hold their values.

	inherited by dbFieldListOfValues, dbFieldReferenceID

	@see dbRecord dbDefinition
*/

// About enum dbFieldDefinition::Flags: http://groups.google.es/group/comp.lang.c++.moderated/browse_thread/thread/cdc0d3aee35e3bfd/ea7136384059df11?lnk=st&q=enum+operator&rnum=19#ea7136384059df11

namespace gong {

dbFieldDefinition::dbFieldDefinition(const dbFieldDefinition &other)
    : mTableName(other.mTableName), mName(other.mName), mSqlColumnType(other.mSqlColumnType),
      mSqlWidth(other.mSqlWidth), mDecimals(other.mDecimals), mCaption(other.mCaption),
      mFlags(other.mFlags), mDescription(other.mDescription), mDefaultValue(other.mDefaultValue),
      mReference(other.mReference), mStyle(other.mStyle), mDisplayWidth(other.mDisplayWidth)
{
}

void dbFieldDefinition::init_class()
{
    _GONG_DEBUG_ASSERT( !mName.isEmpty() );
    if( mName.endsWith("_ID") )
        setIsReference( true );
    if( isSequence() )
        setPrimaryKey( true );
    if( isPrimaryKey() )
        setUnique( true );
    if( mDescription.isEmpty() && mCaption.isEmpty() ) {
        mCaption = mName.lower().proper();
        mDescription = mCaption;
    } else if( mCaption.isEmpty() ) {
        mCaption = mDescription;
    } else {
        mDescription = mCaption;
    }
    if( mSqlWidth == 0 ) {
        switch( mSqlColumnType ) {
        case SQLINTEGER:
            mDisplayWidth = 10;
            break;
        case SQLDECIMAL:
            mDisplayWidth = 12;
            break;
        case SQLDATE:
        case SQLTIME:
            mDisplayWidth = 8;
            break;
        case SQLDATETIME:
            mDisplayWidth = 17;
            break;
        case SQLFLOAT:
            mDisplayWidth = 12;
            break;
        case SQLBOOL:
            mDisplayWidth = 4;
            break;
        case SQLSTRING:
        case SQLTEXT:
        case SQLBLOB:
        default:
            mDisplayWidth = 100;
            break;
        }
    } else {
        mDisplayWidth = mSqlWidth;
    }
}

dbFieldDefinition* dbFieldDefinition::setSqlWidth( const unsigned& sqlwidth )
{
    mDisplayWidth = mSqlWidth = sqlwidth;
    return this;
}

Xtring dbFieldDefinition::extractFieldName(const Xtring &fullfldname)
{
    if( fullfldname.find(".") != Xtring::npos ) {
        return fullfldname.mid(fullfldname.find(".") + 1);
    } else {
        return fullfldname;
    }
}

Xtring dbFieldDefinition::extractTableName(const Xtring &fullfldname)
{
    if( fullfldname.find(".") != Xtring::npos ) {
        return fullfldname.left( fullfldname.find(".") );
    } else {
        return Xtring();
    }
}

Xtring dbFieldDefinition::sameSQLSchema( const dbFieldDefinition *other, dbConnection *conn,
        bool purging ) const
{
    Xtring ret;
    if( getName().upper() != other->getName().upper() )
        ret = "ALTER TABLE " + getTableName() + " ADD COLUMN " + getName() + " " + toDDL(conn) + ";\n";
    else if( getName().upper() == "ID" ) {
        ; // Ids are defined very differently in each dbrms
    } else if( (getName() != other->getName()) // Puede que hayan cambiado las mayusculas del nombre
               || ( getSqlColumnType() != other->getSqlColumnType() )
               || ( isPrimaryKey() != other->isPrimaryKey() )
               || ( isSequence() != other->isSequence() )
             )  {
        _GONG_DEBUG_PRINT(3, Xtring::printf("Column type: %d, %d", getSqlColumnType(), other->getSqlColumnType() ) );
        _GONG_DEBUG_PRINT(3, Xtring::printf("Primary key: %d, %d", isPrimaryKey(), other->isPrimaryKey() ) );
        _GONG_DEBUG_PRINT(3, Xtring::printf("Sequence: %d, %d", isSequence(), other->isSequence() ) );
        ret ="ALTER TABLE " + getTableName() + " CHANGE COLUMN " + other->getName() + " " + getName() + " " + toDDL(conn) + ";\n";
    }
    else if( !purging ) {
        if( canBeNull() != other->canBeNull() ) {
            _GONG_DEBUG_PRINT(3, Xtring::printf("Null: %d, %d", canBeNull(), other->canBeNull() ) );
            ret ="ALTER TABLE " + getTableName() + " CHANGE COLUMN " + other->getName() + " " + getName() + " " + toDDL(conn) + ";\n";
        } else if( getSqlColumnType() != SQLFLOAT && // Float fields have no size
                   ( ((getSqlWidth() != 0) && (other->getSqlWidth() != 0) && (getSqlWidth() != other->getSqlWidth() ))
                     || ( getDecimals() != other->getDecimals() ) ) ) {
            ret ="ALTER TABLE " + getTableName() + " CHANGE COLUMN " + other->getName() + " " + getName() + " " + toDDL(conn) + ";\n";
        } else if( getDefaultValue() != other->getDefaultValue() ) {
            bool changed = false;
            if( getSqlColumnType() == SQLDATE ) {
                if( getDefaultValue().isEmpty() && other->getDefaultValue() != "0000-00-00" /*mysql*/
                        && other->getDefaultValue() != "0" /*sqlite3*/ )
                    changed = true;
                else if( other->getDefaultValue().isEmpty() && getDefaultValue() != "0000-00-00" /*mysql*/
                         && getDefaultValue() != "0" /*sqlite3*/ )
                    changed = true;
            }
            if( changed ) {
                // Avoid changing column if one default value is empty and the other is "0"
                _GONG_DEBUG_PRINT(3, Xtring::printf("Default value: %s, %s", getDefaultValue().c_str(), other->getDefaultValue().c_str() ) );
                ret ="ALTER TABLE " + getTableName() + " CHANGE COLUMN " + other->getName() + " " + getName() + " " + toDDL(conn) + ";\n";
            }
        }
    }
    if( purging && !isUnique() && other->isUnique() ) {
        _GONG_DEBUG_PRINT(3, Xtring::printf("Unique: %d, %d", isUnique(), other->isUnique() ) );
        ret += "ALTER TABLE " + getTableName() + " DROP INDEX " + getName() + ";\n";
    }
    return ret;
}

bool dbFieldDefinition::isValid( dbRecord *r, dbFieldValue *value,
                                 ValidResult::Context context, ValidResult *integres) const
{
    bool ret = true;
    // Dont fail if !canBeNull and the integer value is 0
    // comboints can get the value 0
    if (!canBeNull() && (
                value->isNull()
                || (value->type() == Variant::tString && value->isEmpty())
                || (value->type() == Variant::tDateTime && value->isEmpty())
                || (value->type() == Variant::tDate && value->isEmpty())
                || (value->type() == Variant::tTime && value->isEmpty())
                || (isCode() && value->isEmpty())
				|| (isReference() && value->toUInt() == 0 )
            )
       ) {
        if( integres ) {
            if( Variant::isNumeric( value->type() ) ) {
                integres->addError( Xtring::printf( _("'%s' no puede ser cero."),
                                                    getCaption().c_str() ), getName() );
            } else {
                integres->addError( Xtring::printf( _("'%s' no puede quedarse vacío."),
                                                    getCaption().c_str() ), getName() );
            }
        }
        ret = false;
    }
    switch( value->type() )
    {
    case Variant::tString:
        if( getSqlWidth() != 0 && value->toString().size() > getSqlWidth() ) {
            if( integres )
                integres->addWarning( Xtring::printf( _("La longitud de '%s' no puede ser mayor de %d letras. Se cortará al grabarlo."),
                                                      getCaption().c_str(), getSqlWidth() ),
                                      getName().c_str() );
        }
        break;
    default:
        break; /// \todo {0.3.4}
    }
    return ret;
}


Variant::Type dbFieldDefinition::getVariantType() const
{
    return dbConnection::toVariantType( getSqlColumnType() );
}

/**
 * Creates a new dbFieldValue from a dbFieldDefinition.
 * If the field can be null, makes it null
 * @return a pointer to the newly created fldValue
 */
dbFieldValue dbFieldDefinition::createFieldValue() const
{
    return dbFieldValue( getVariantType(), canBeNull(), this );
}

Xtring dbFieldDefinition::toSQL( dbConnection *conn, const dbFieldValue &value, bool inserting) const
{
    switch( getSqlColumnType() ) {
    case SQLTEXT:
    case SQLSTRING:
    case SQLBLOB:
        return conn->toSQL( value.toString() );
    case SQLINTEGER:
        return conn->toSQL( value.toInt() );
    case SQLDECIMAL:
        return conn->toSQL( value.toMoney() );
    case SQLDATE:
        return conn->toSQL( value.toDate() );
    case SQLTIMESTAMP:
    case SQLDATETIME:
        return conn->toSQL( value.toDateTime() );
    case SQLTIME:
        return conn->toSQL( value.toTime() );
    case SQLBOOL:
        return conn->toSQL( value.toBool()?"1":"0" );
    case SQLFLOAT:
    default:
        return conn->toSQL( value.toDouble() );
    }
}


/**
 * Creates the DDL definition for this field.
 * Primary keys are created in dbTableDefinition::create, because they can contain more than one field
 * @param conn
 * @return
 */
Xtring dbFieldDefinition::toDDL( dbConnection *conn) const
{
    _GONG_DEBUG_ASSERT( conn );
    Xtring ddl;
    switch( getSqlColumnType() ) {
    case SQLTEXT:
        ddl = "MEDIUMTEXT";
        break;
    case SQLSTRING:
        ddl = "VARCHAR(" + Xtring::number(mSqlWidth?mSqlWidth:100) + ")";
        break;
    case SQLINTEGER:
        if( conn->isSQLite() && isPrimaryKey() )
            ddl = "INTEGER";
        else
            ddl = "INTEGER(" + Xtring::number(mSqlWidth?mSqlWidth:10) + ")";
        break;
    case SQLDECIMAL:
        ddl = "DECIMAL(" + Xtring::number(mSqlWidth?mSqlWidth:10) + "," + Xtring::number(mDecimals) + ")";
        break;
    case SQLDATE:
        ddl = "DATE";
        break;
    case SQLTIMESTAMP:
        ddl = "TIMESTAMP";
        break;
    case SQLDATETIME:
        ddl = "DATETIME";
        break;
    case SQLTIME:
        ddl = "TIME";
        break;
    case SQLBOOL:
        ddl = "BOOL";
        break;
    case SQLFLOAT:
        ddl = "FLOAT";
        break;
    case SQLBLOB:
        ddl = "LONGBLOB";
        break;
    }
    // PostGres serials are unique and not null by default
    /* In mysql and sqlite3, sequences must be primary keys, and can only consist of one field */
    if( isPrimaryKey() )
        ddl += " PRIMARY KEY";
    if( isSequence() )
    {
        if( conn->isMySQL() )
            ddl += " AUTO_INCREMENT";
        else if( conn->isPGSQL() )
            ddl += " SERIAL";
        else if( conn->isSQLite() )
            ddl += " AUTOINCREMENT";
    } else if( getDefaultValue().isEmpty() && !canBeNull() ) {
        switch( getSqlColumnType() ) {
        case SQLTEXT:
        case SQLSTRING:
        case SQLBLOB:
            ddl += " DEFAULT ''";
            break;
        case SQLINTEGER:
        case SQLDECIMAL:
        case SQLDATE:
        case SQLDATETIME:
        case SQLTIME:
        case SQLBOOL:
        case SQLFLOAT:
            ddl += " DEFAULT 0";
            break;
        case SQLTIMESTAMP:
            ddl += " DEFAULT CURRENT_TIMESTAMP";
            break;
        }
    } else if( !canBeNull() ) {
        switch( getSqlColumnType() ) {
        case SQLTEXT:
        case SQLSTRING:
        case SQLBLOB:
        case SQLDATE:
        case SQLTIMESTAMP:
        case SQLDATETIME:
        case SQLTIME:
            if( conn )
                ddl += " DEFAULT " + conn->toSQL(getDefaultValue());
            else
                ddl += " DEFAULT '" +  getDefaultValue() + "'";
            break;
        case SQLINTEGER:
        case SQLBOOL: // es un entero
        case SQLFLOAT:
        case SQLDECIMAL:
            ddl += " DEFAULT " + getDefaultValue();
            break;
        }
    }
    if( !canBeNull() || isSequence() || isPrimaryKey() ) // Unique indexes can be null
        ddl += " NOT NULL";
    else
        ddl += " NULL";
    // Other clauses
#if 0
    /* Unique indexes are created later by createIndexes */
    if( isUnique() )
        ddl += " UNIQUE";
#endif
    return ddl;
}

#ifdef _GONG_DEBUG
std::ostream &operator<<(std::ostream &out, const dbFieldDefinition &flddef)
{
    out << "DBDEF.FIELD." << flddef.getFullName() << ".CAPTION(string)=\"" << flddef.getCaption() << "\";" << std::endl;
    out << "DBDEF.FIELD." << flddef.getFullName() << ".DESCRIPTION(string)=\"" << flddef.getDescription() << "\";" << std::endl;
    out << "DBDEF.FIELD." << flddef.getFullName() << ".WIDTH(int)=\"" << flddef.getDisplayWidth() << "\";" << std::endl;
    out << "DBDEF.FIELD." << flddef.getFullName() << ".DEFAULTVALUE(string)=\"" << flddef.getDefaultValue() << "\";" << std::endl;
    out << "DBDEF.FIELD." << flddef.getFullName() << ".STYLE(string)=\"" << flddef.getStyle() << "\";" << std::endl;
    out << "DBDEF.FIELD." << flddef.getFullName() << ".CANBENULL(bool)=\"" << (flddef.canBeNull() ? "true":"false") << "\";" << std::endl;
    out << "DBDEF.FIELD." << flddef.getFullName() << ".VISIBLE(bool)=\"" << (flddef.isVisible() ? "true":"false") << "\";" << std::endl;
    out << "DBDEF.FIELD." << flddef.getFullName() << ".READONLY(bool)=\"" << (flddef.isReadOnly() ? "true":"false") << "\";" << std::endl;
#if 0
    out << "\t\t" << Xtring(flddef.getName()).pad(20) << '\t'
        << flddef.toDDL(0) << std::endl;
#endif
    return out;
}
#endif

Variant dbFieldDefinition::customDefaultValue() const
{
    return mDefaultValue;
}

/** Redefine if your field definition adds some extra data */
dbFieldDefinition *dbFieldDefinition::clone() const
{
    return new dbFieldDefinition( *this );
}

Xtring dbFieldDefinition::getDisplayValue( const Variant& value ) const
{
    return value.toString();
}

int dbFieldDefinition::getStyleWidth( const dbDefinition* pdb ) const
{
    int w = 0;
    if( !mStyle.isEmpty() ) {
        dbFieldStyle *fldstyle = pdb->findFieldStyle( mStyle );
        if( fldstyle )
            w = fldstyle->getWidth();
    }
    if( !w )
        w = getDisplayWidth();
    if( !w )
        w = getSqlWidth();
    return w;
}


dbRelationDefinition* dbFieldDefinition::findRelationDefinition(const dbTableDefinition &tbldef) const
{
    for( dbRelationDefinitionDict::const_iterator it = tbldef.getRelationDefinitions().begin();
            it != tbldef.getRelationDefinitions().end(); ++it ) {
        dbRelationDefinition *reldef = it->second;
        if( reldef->getLeftField() == getName() )
            return reldef;
    }
    _GONG_DEBUG_WARNING( Xtring::printf("Relation not found for field '%s' in table '%s'",
                                        getName().c_str(), tbldef.getName().c_str() ) );
    return 0;
}


} // namespace gong
