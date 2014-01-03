/**
	@class gong::dbTableDefinition

	@brief Table or view definition for the data dictionary

	A table definition consists of a set of field, index and relation definitions.
	Note that a view definition is just a typedef'd table definition, although without indexes or relations.

	You can create table definition in several ways:
	- With the constructor that takes all the possible properties as parameters and then adding fields,
		indexes and relations,
	- From a SELECT statement, retrieving the information from an existing data dictionary
		(specially for views) with the second constructor
	- From an existing SQL database with \a fromSQLSchema

	To add fields, you can use any of the several addFieldXXXXXX functions
	To add indexes, use \a addIndex
	To add relations, use \a addRelation

	Once defined, you can:
	- Add it to the root data dictionary with \a dbDefinition::addTable
	- \a create the table or \a createIndexes and \a dropIndexes from the underlying database engine.

	The fields are stored in a \a Dictionary whose keys are the full name of the field, that is to say, TABLE.FIELD.
	That is to avoid conflicts with fields from different tables when defining views.

	@see dbDefinition

	\todo {add} Calculated fields
	\todo {refactor} dbFieldPassword, dbFieldFilename
	\todo {autodelete} delete field and index definitions
	\todo {refactor} change Femenina into Femenin?
 */

#include <memory> // auto_ptr<>
#include "gongdebug.h"
#include "gongdbfieldlistofvalues.h"
#include "gongdbresultset.h"
#include "gongdbdefinition.h"
#include "gongdbtabledefinition.h"
#include "gongdbrecordbehavior.h"

namespace gong {

dbTableDefinition::dbTableDefinition( dbDefinition &db, const Xtring& name,
                                      const Xtring& from, const Xtring& where,
                                      const Xtring& orderby, const Xtring& groupby )
    : mdbDefinition( db ), mName( name ), mFemenina( true ), mTemporary( false ),
    mFrom( from ), mWhere( where ), mOrderBy( orderby ), mGroupBy( groupby )
{
    if ( mDescSingular.isEmpty() )
        mDescSingular = name.proper();
    if ( mDescPlural.isEmpty() )
        mDescPlural = name.proper() + "s";
}

dbTableDefinition::dbTableDefinition( const dbTableDefinition &other )
    : mdbDefinition( other.mdbDefinition ), mName( other.mName ), mDescPlural( other.mDescPlural ), mDescSingular( other.mDescSingular ),
      mFemenina( other.mFemenina ), mFrom( other.mFrom ), mWhere( other.mWhere ), mOrderBy( other.mOrderBy ),
      mGroupBy( other.mGroupBy ), mFldIDName( other.mFldIDName )
{
    // We use the integer iterator because we don't want them in alpha order
    for( std::size_t f = 0; f < other.mFieldDefinitions.size(); f++ ) {
        dbFieldDefinition *flddefcloned = other.mFieldDefinitions[f]->clone();
//        dbFieldDefinition *flddefcloned = new dbFieldDefinition( *other.mFieldDefinitions[f] );
		mFieldDefinitions.insert( other.mFieldDefinitions.getKey( f ), flddefcloned );
    }
    for( std::size_t i = 0; i < other.mIndexDefinitions.size(); i++ ) {
        dbIndexDefinition *idxdef = new dbIndexDefinition( *other.mIndexDefinitions[i] );
        mIndexDefinitions.push_back( idxdef );
    }
    for( std::size_t r = 0; r < other.mRelationDefinitions.size(); r++ ) {
		dbRelationDefinition *reldef = new dbRelationDefinition( *other.mRelationDefinitions[r] );
		mRelationDefinitions.insert( other.mRelationDefinitions.getKey(r), reldef );
	}
}

dbTableDefinition::~dbTableDefinition()
{
    uint i;
    for ( i = 0; i < mRelationDefinitions.size(); i++ )
        delete mRelationDefinitions[i];
    for ( i = 0; i < mIndexDefinitions.size(); i++ )
        delete mIndexDefinitions[i];
    for ( i = 0; i < mFieldDefinitions.size(); i++ )
        delete mFieldDefinitions[i];
}

void dbTableDefinition::clearFields()
{
    mFieldDefinitions.clear();
}

const Xtring& dbTableDefinition::getFldIDName() const
{
    if( mFldIDName.isEmpty() ) {
        _GONG_DEBUG_WARNING( "Table '" + getName() + "' has not an ID field" );
    }
    return mFldIDName;
}

dbIndexDefinition *dbTableDefinition::addIndex( const dbIndexDefinition *idxdef )
{
    mIndexDefinitions.push_back( const_cast<dbIndexDefinition *>( idxdef ) );
    return const_cast<dbIndexDefinition *>( idxdef );
}


dbIndexDefinition *dbTableDefinition::addIndex( const dbFieldDefinition *flddef )
{
    dbIndexDefinition *idxdef = new dbIndexDefinition( flddef->getName(), flddef->isUnique(), flddef->getFlags() );
    idxdef->addField( flddef->getName() );
    return addIndex( idxdef );
}

dbIndexDefinition *dbTableDefinition::deleteIndex( const char *name )
{
    for( dbIndexDefinitionsList::iterator idxit = mIndexDefinitions.begin();
            idxit != mIndexDefinitions.end();
            ++ idxit ) {
        if( (*idxit)->getName() == Xtring( name ) ) {
            mIndexDefinitions.erase( idxit );
            return *idxit;
        }
    }
    _GONG_DEBUG_WARNING( Xtring("The index ") + name + " does not exist in the table " + getName() );
    return 0;
}

dbIndexDefinition* dbTableDefinition::addMultipleIndex( const char* name,
        const Xtring& fields, bool unique )
{
    dbIndexDefinition *index = new dbIndexDefinition( name, true );
    XtringList lstfields;
    fields.tokenize( lstfields, "," );
    for( XtringList::const_iterator it = lstfields.begin();
            it != lstfields.end();
            ++ it ) {
        index->addField( *it );
    }
    return addIndex(index->setUnique( unique ) );
}

void dbTableDefinition::clearIndexes()
{
    mIndexDefinitions.clear();
}

bool dbTableDefinition::addBehavior(dbRecordBehavior* behavior)
{
    _GONG_DEBUG_ASSERT( behavior );
    if( behavior->addTo( this ) ) {
        mRecordBehaviors.push_back( behavior );
        return true;
    } else {
        return false;
    }
}



// Podemos estar añadiendo el campo a una tabla o a una vista, por lo tanto la tabla del campo puede ser distinta a esta tabla
dbFieldDefinition *dbTableDefinition::addField( const dbFieldDefinition *fielddef )
{
    if ( !mFieldDefinitions.insert( fielddef->getFullName(), ( dbFieldDefinition * )fielddef ) )
        _GONG_DEBUG_WARNING( Xtring::printf( "Table '%s' already has a field named '%s'", getName().c_str(), fielddef->getFullName().c_str() ) );
    return const_cast<dbFieldDefinition *>( fielddef );
}

// Este campo se añade sin referencia a una tabla, por lo tanto pertencece a esta tabla
dbFieldDefinition *dbTableDefinition::addField( const Xtring& name, SqlColumnType sqltype,
        unsigned int width, unsigned short int decimals,
        dbFieldDefinition::Flags flags, const Xtring& defaultvalue )
{
    dbFieldDefinition *fld = new dbFieldDefinition( getName(), name, sqltype, width, decimals,
            flags, defaultvalue, this );
    if ( !mFieldDefinitions.insert( getName() + "." + name, fld ) )
        _GONG_DEBUG_WARNING( Xtring::printf( "Table '%s' already has a field named '%s'", getName().c_str(), name.c_str() ) );
    return fld;
}

bool dbTableDefinition::deleteField(const Xtring &name)
{
    dbFieldDefinition *flddef = findFieldDefinition( name );
    if( flddef ) {
        mFieldDefinitions.erase( getFullFldName(name) );
        delete flddef;
        return true;
    }
    return false;
}

bool dbTableDefinition::deleteField(const dbFieldDefinition* fielddef)
{
    for( dbFieldDefinitionsList::iterator it = mFieldDefinitions.begin();
            it != mFieldDefinitions.end(); ++it ) {
        if( it->second == fielddef ) {
            mFieldDefinitions.erase( it );
            return true;
        }
    }
    return false;
}


dbFieldDefinition *dbTableDefinition::addFieldRecordID( const Xtring &idname )
{
    Xtring idfld = ( idname == Xtring::null ? ( mFldIDName == Xtring::null ? "ID" : mFldIDName ) : idname );
    mFldIDName = idfld;
    return addField( idfld, SQLINTEGER, 11, 0,
                     dbFieldDefinition::Flags( dbFieldDefinition::PRIMARYKEY | dbFieldDefinition::SEQUENCE |
                             dbFieldDefinition::NOTNULL | dbFieldDefinition::INVISIBLE ) );
}

/**
 * There will be only an unique index, with all the fields that have been declared as unique
 * @param conn
 * @param ignoreerrors
 * @return
 */
bool dbTableDefinition::create( dbConnection *conn, bool ifnotexists,
                                const Xtring &theextraargs, bool ignoreerrors )
{
    Xtring extraargs = theextraargs;
    if( extraargs.find( "TEMPORARY" ) != Xtring::npos )
        extraargs.replace( "TEMPORARY", "" );
    if( mTemporary )
        extraargs += " TEMPORARY";
    Xtring fieldsdef, tabledef = "CREATE " + extraargs + " TABLE ";
    if ( conn->isMySQL() && ifnotexists )
        tabledef += "IF NOT EXISTS ";
    tabledef += conn->nameToSQL( getName() ) + "(";
    for ( unsigned int i = 0; i < getFieldDefinitions().size(); i++ ) {
        dbFieldDefinition *flddef = getFieldDefinition( i );
        if ( flddef->isCalculated() )
            continue;
        if ( !fieldsdef.isEmpty() )
            fieldsdef += ", ";
        fieldsdef += conn->nameToSQL( flddef->getName() ) + ' ' + flddef->toDDL( conn );
    }
    tabledef += fieldsdef + ")";
    return conn->exec( tabledef );
}

bool dbTableDefinition::createIndexes( dbConnection *conn, bool ignoreerrors )
{
    for ( unsigned int i = 0; i < getFieldDefinitions().size(); i++ ) {
        dbFieldDefinition *flddef = getFieldDefinition( i );
        if ( flddef->isUnique() && !flddef->isPrimaryKey() ) {
            conn->exec( "ALTER TABLE " + getName() + " ADD UNIQUE INDEX (" + flddef->getName() + ")", true );
        }
    }
    for ( unsigned int i = 0; i < getIndexDefinitions().size(); i++ ) {
        dbIndexDefinition *idxdef = getIndexDefinition( i );
        idxdef->create( conn, this, ignoreerrors );
    }
    return true;
}

/**
 * Removes all indexes that might have been created by gong.
 * These are, the automatic UNIQUE ones, the ones added by dbIndexDefinitions
 * 		and any one that could remain which has _gong_ prepended to its name
 * @param conn One active connection
 * @param ignoreerrors Whether errors will be ignored
 * @return  Always true
 */
bool dbTableDefinition::dropIndexes( dbConnection *conn, bool removeall, bool ignoreerrors )
{
    for ( unsigned int i = 0; i < getFieldDefinitions().size(); i++ ) {
        dbFieldDefinition *flddef = getFieldDefinition( i );
        if ( flddef->isUnique() && !flddef->isPrimaryKey() ) {
            conn->exec( "ALTER TABLE " + getName() + " DROP INDEX " + flddef->getName(), true );
        }
    }
    for ( unsigned int i = 0; i < getIndexDefinitions().size(); i++ ) {
        dbIndexDefinition *idxdef = getIndexDefinition( i );
        idxdef->drop( conn, this, ignoreerrors );
    }
    // Remove any remaining index with _gong_ prefix
    dbResultSet *rsIndexes = conn->select( "SHOW INDEXES FROM " + getName() );
    while( rsIndexes->next() ) {
        Xtring idxname = rsIndexes->toString( 2 );
        if (( removeall || idxname.startsWith( _GONG_INDEX_PREFIX ) ) && idxname != "PRIMARY" )
            conn->exec( "ALTER TABLE " + getName() + " DROP INDEX " + idxname, ignoreerrors );
    }
    /// FIXME: {autodelete} Check if this resultsets are deleted when an exception is issued
    delete rsIndexes;
    return true;
}


dbTableDefinition *dbTableDefinition::fromSQLSchema( dbConnection *conn,
        dbDefinition &db,
        const Xtring &tblname )
{
    Xtring fldname;
    SqlColumnType t;
    unsigned short int w, d;
    dbTableDefinition *tbldef = 0;

    if ( conn->isMySQL() ) {
        std::auto_ptr<dbResultSet> rsFields( conn->select( "SHOW FIELDS FROM " + tblname ) );
        tbldef = new dbTableDefinition( db, tblname );
        tbldef->setDescSingular( db.getName() + "." + tblname + " from SQL Schema" );
        tbldef->mFrom = tblname;
        while( rsFields->next() ) {
            fldname = rsFields->toString( 0 );
            //		const char *column_type = rsFields->at(nrow).toString(1).c_str();
            t = dbConnection::extractSqlColumnType( rsFields->toString( 1 ).c_str(), &w, &d );
            dbFieldDefinition::Flags tmpflags = dbFieldDefinition::NONE;
            if ( rsFields->toString( 2 ) == "NO" )
                tmpflags |= dbFieldDefinition::NOTNULL;
            if ( rsFields->toString( 3 ) == "UNI" )
                tmpflags |= dbFieldDefinition::UNIQUE;
            if ( rsFields->toString( 3 ) == "PRI" )
                tmpflags |= dbFieldDefinition::PRIMARYKEY;
            if ( rsFields->toString( 5 ) == "auto_increment" )
                tmpflags |= dbFieldDefinition::SEQUENCE;
            dbFieldDefinition *flddef = new dbFieldDefinition(
                tblname, fldname, t, w, d,
                tmpflags, // null, primarykey, ...
                rsFields->toString( 4 ) // default value
            );
            flddef->setCaption( fldname.proper() );
            flddef->setDescription( tblname + "." + fldname );
            tbldef->addField( flddef );
        }
    }
    return tbldef;
}

Xtring dbTableDefinition::sameSQLSchema( const dbTableDefinition *other, dbConnection *conn, bool purging ) const
{
    Xtring ret;
    unsigned int nf, nfother;
    // Check fields that are not in the other one, to add them
    for ( nf = 0; nf < getFieldDefinitions().size(); nf++ ) {
        const dbFieldDefinition *flddef = getFieldDefinition( nf );
        bool existe = false;
        for ( nfother = 0; nfother < other->getFieldCount(); nfother ++ ) {
            if ( flddef->getName().upper() == other->getFieldDefinition( nfother )->getName().upper() ) {
                existe = true;
                if ( !flddef->isCalculated() )
                    ret += flddef->sameSQLSchema( other->getFieldDefinition( nfother ), conn, purging );
                break;
            }
        }
        if ( !existe && !flddef->isCalculated() )  {
            ret += "ALTER TABLE " + getName() + " ADD COLUMN "
                   + flddef->getName() + " " + flddef->toDDL( conn );
            if ( nf == 0 )
                ret += " FIRST;\n";
            else
                ret += " AFTER " + getFieldDefinition( nf - 1 )->getName() + ";\n";
        }
    }
    if( purging ) {
        // Check fields in the other one that are not in this one
        for ( nfother = 0; nfother < other->getFieldCount(); nfother ++ ) {
            bool existe = false;
            for ( nf = 0; nf < getFieldDefinitions().size(); nf++ ) {
                if ( getFieldDefinition( nf )->getName().upper() == other->getFieldDefinition( nfother )->getName().upper() ) {
                    if ( !getFieldDefinition( nf )->isCalculated() )
                        existe = true;
                    break;
                }
            }
            if ( !existe /*&& !other->getFieldDefinition(nfother)->isCalculated()*/ )
                ret += "ALTER TABLE " + getName() + " DROP COLUMN "
                       + other->getFieldDefinition( nfother )->getName() + ";\n";
        }
    }
    return ret;
}

dbRelationDefinition *dbTableDefinition::addRelation( const dbRelationDefinition::Type& type,
        const Xtring& lefttable, const Xtring& leftfield, const Xtring& righttable, const Xtring& rightfield )
{
    dbRelationDefinition *reldef = new dbRelationDefinition( type, lefttable, leftfield, righttable, rightfield );
    mRelationDefinitions.insert( reldef->getName(), reldef );
    return reldef;
}


Xtring dbTableDefinition::getFullFldName( const Xtring &fldname ) const
{
    if ( fldname.find( "." ) != Xtring::npos )
        return fldname;
    else
        return getName() + "." + fldname;
}


int dbTableDefinition::getFieldPosition( const Xtring &fldname ) const
{
    for ( unsigned int i = 0; i < getFieldDefinitions().size(); i++ ) {
        const dbFieldDefinition *flddef = getFieldDefinition( i );
        if ( flddef->getName().upper() == fldname.upper() )
            return i;
    }
    _GONG_DEBUG_WARNING( Xtring::printf( "%s: Field %s not found in table %s",
                                         __FUNCTION__, fldname.c_str(), getName().c_str() ) );
    return -1;
}


bool dbTableDefinition::parseSelectStatement( const Xtring &origselect,
        Xtring &fields, Xtring &from,
        Xtring &where, Xtring &orderby,
        Xtring &groupby )
{
    fields.clear();
    from.clear();
    where.clear();
    orderby.clear();
    groupby.clear();
    Xtring select = origselect;
    select.replace( "\t", " " ).replace( "\n", " " ).replace( "\r", " " );

    Xtring::size_type posselect = select.find( "SELECT " );
    if ( posselect == Xtring::npos )
        return false;
    Xtring::size_type posfrom = select.find( " FROM " );
    if ( posfrom == Xtring::npos )
        return false;
    fields = select.mid( posselect + 7, posfrom - posselect - 7 );
    Xtring::size_type poswhere = select.find( " WHERE ", posfrom );
    bool haswhere = false;
    if ( poswhere != Xtring::npos ) {
        from = select.mid( posfrom + 6, poswhere - posfrom - 6 );
        haswhere = true;
    } else poswhere = posfrom - 1;
    Xtring::size_type posgroupby = select.find( " GROUP BY ", posfrom );
    Xtring::size_type posorderby = select.find( " ORDER BY ", posfrom );
    if ( posgroupby == Xtring::npos && posorderby == Xtring::npos ) {
        if ( haswhere )
            where = select.mid( poswhere + 7 );
        else
            from = select.mid( posfrom + 6 );
    } else if ( posgroupby == Xtring::npos ) {
        ( haswhere ? where : from ) = select.mid( poswhere + 7, posorderby - poswhere - 7 );
        orderby = select.mid( posorderby + 10 );
    } else if ( posorderby == Xtring::npos ) {
        ( haswhere ? where : from ) = select.mid( poswhere + 7, posgroupby - poswhere - 7 );
        groupby = select.mid( posgroupby + 10 );
    } else {
        if ( posgroupby < posorderby ) {
            ( haswhere ? where : from ) = select.mid( poswhere + 7, posgroupby - poswhere - 7 );
            groupby = select.mid( posgroupby + 10, posorderby - posgroupby - 10 );
            orderby = select.mid( posorderby + 10 );
        } else {
            ( haswhere ? where : from ) = select.mid( poswhere + 7, posorderby - poswhere - 7 );
            orderby = select.mid( posorderby + 10, posgroupby - posorderby - 10 );
            groupby = select.mid( posgroupby + 10 );
        }
    }
    fields.trim();
    from.trim();
    where.trim();
    groupby.trim();
    orderby.trim();
    return true;
}

dbFieldDefinition *dbTableDefinition::findFieldByFlags( const dbFieldDefinition::Flags & flags ) const
{
    for ( unsigned int i = 0; i < getFieldDefinitions().size(); i++ ) {
        dbFieldDefinition *flddef = getFieldDefinition( i );
        if (( flddef->getFlags() & flags ) == flags )
            return flddef;
    }
    return 0;
}

dbFieldDefinition *dbTableDefinition::findFieldDefinition( const Xtring &name, bool warning ) const
{
    dbFieldDefinition *flddef = mFieldDefinitions[getFullFldName( name )];
    if ( !flddef && warning )
        _GONG_DEBUG_WARNING( Xtring::printf( "Field '%s' not found", getFullFldName( name ).c_str() ) );
    return flddef;
}

dbFieldDefinition *dbTableDefinition::getFieldDefinition( unsigned int i ) const
{
    return mFieldDefinitions[i];
}

#ifdef _GONG_DEBUG
std::ostream &operator<<( std::ostream &out, const dbTableDefinition &tbldef )
{
    unsigned int i;
    out << "DBDEF.TABLE." << tbldef.getName() << ".DESC_SINGULAR(string)=\"" << tbldef.getDescSingular() << "\";" << std::endl;
    out << "DBDEF.TABLE." << tbldef.getName() << ".DESC_PLURAL(string)=\"" << tbldef.getDescPlural() << "\";" << std::endl;
    out << "DBDEF.TABLE." << tbldef.getName() << ".FEMENINA(bool)=\"" << ( tbldef.isFemenina() ? "true" : "false" ) << "\";" << std::endl;
    for ( i = 0; i < tbldef.getFieldDefinitions().size(); i++ )
        out << *( const_cast<dbTableDefinition &>( tbldef ).getFieldDefinitions()[i] );
    out << "\t\tRelations" << std::endl;
    for ( i = 0; i < tbldef.getRelationDefinitions().size(); i++ )
        out << *( const_cast<dbTableDefinition &>( tbldef ).getRelationDefinitions()[i] );
    out << "\t\tIndexes" << std::endl;
    for ( i = 0; i < tbldef.getIndexDefinitions().size(); i++ )
        out << *( const_cast<dbTableDefinition &>( tbldef ).getIndexDefinitions()[i] );
    return out;
}
#endif

}; // namespace



