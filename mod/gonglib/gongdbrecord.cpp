#include <memory> // auto_ptrread<>

#include "gongformatter.h"
#include "gonglibrary.h"
#include "gongdbrecord.h"
#include "gongdbrecordlist.h"
#include "gongdbrecordbehavior.h"

namespace gong {

dbRecord::dbRecord ( dbConnection *conn, const dbTableDefinition *tbldef, dbRecordID recid, dbUser *puser )
    : pTableDef ( tbldef ), pUser ( puser ), pConn ( conn ), mIsNew ( true ), mIsRead ( false ), mIsDeleted( false )
{
    _GONG_DEBUG_ASSERT ( pConn );
    _GONG_DEBUG_ASSERT ( tbldef );
    init_record();
    setRecordID( recid );
}

/// \todo {refactor} No entiendo por qué tengo que definir estas dos funciones, una const y la otra non-const
// Preguntar en la lista.

dbRecord::dbRecord ( dbConnection *conn, dbTableDefinition *tbldef, dbRecordID recid, dbUser *puser )
    : pTableDef ( tbldef ), pUser ( puser ), pConn ( conn ), mIsNew ( true ), mIsRead ( false ), mIsDeleted( false )
{
    _GONG_DEBUG_ASSERT ( tbldef );
    init_record();
    setRecordID( recid );
}

dbRecord::~dbRecord()
{
    _GONG_DEBUG_PRINT(10, "Destroying record for table " + pTableDef->getName() );
    removeRelations();
}

void dbRecord::init_record()
{
	mErrors.clear();
    // Create values
    for ( unsigned int i = 0; i<pTableDef->getFieldCount(); i++ )
    {
        const dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( i );
		dbFieldValue value(flddef->createFieldValue());
        mFieldValues.insert( flddef->getName(), value );
        mOrigFieldValues.insert( flddef->getName(), value );
		Xtring s(value.toString());
		Xtring s1(flddef->createFieldValue().toString());
//  		_GONG_DEBUG_PRINT(2, Xtring::printf("Setting origvalue[%d]%s(%s)=%s(%s)",
//  											i, flddef->getName().c_str(), Variant::typeToName( mOrigFieldValues.seq_at(i).type() ),
//  											value.toString().c_str(), Variant::typeToName( value.type() ) ) );
    }
    // Create relations
    for ( unsigned int nr = 0; nr < getTableDefinition()->getRelationDefinitions().size(); ++nr )
    {
        dbRelationDefinition *reldef = getTableDefinition()->getRelationDefinitions().seq_at(nr);
        dbRecordRelation *rel;
//  		_GONG_DEBUG_PRINT(0, Xtring::printf ( "Creating relation %s.%s->%s.%s",
//  		                                        reldef->getLeftTable().c_str(), reldef->getLeftField().c_str(),
//  		                                        reldef->getRightTable().c_str(), reldef->getRightField().c_str() ) );
        // Lazy creation of related records
        if ( reldef->getType() == dbRelationDefinition::one2many
                || reldef->getType() == dbRelationDefinition::many2many )
            rel = new dbRecordRelation ( reldef, true, 0, new dbRecordList() );
        else
            rel = new dbRecordRelation ( reldef, true, 0, 0 );
        mRecordRelations.insert ( rel->getName(), rel );
    }
}

// Virtual constructor
dbRecord *dbRecord::create( dbConnection *conn, const dbTableDefinition *tbldef,
                            dbRecordID recid, dbUser *user )
{
    _GONG_DEBUG_ASSERT( tbldef );
    _GONG_DEBUG_ASSERT( conn );
    dbRecord *rec = new dbRecord ( conn, tbldef, recid, user );
    rec->init();
    return rec;
}

dbRecordID dbRecord::getRecordID() const
{
    Xtring idfldname( getTableDefinition()->getFldIDName() );
    if( idfldname.isEmpty() )
        idfldname = "ID";
    if( mFieldValues.find(idfldname) != mFieldValues.end() )
        return mFieldValues[idfldname].toUInt();
    return 0;
}

Xtring dbRecord::getIDSqlCond( dbRecordID recid ) const
{
    return getConnection()->nameToSQL( getTableName() )
           + "." + getConnection()->nameToSQL( getTableDefinition()->getFldIDName() )
           + "=" + getConnection()->toSQL( recid ? recid : getRecordID() );
}

Xtring dbRecord::getIDSqlCond(List< dbRecordID > recids) const
{
    return getConnection()->nameToSQL( getTableName() )
           + "." + getConnection()->nameToSQL( getTableDefinition()->getFldIDName() )
           + " IN(" + recids.join(",") + ")";
}

/**
 * @brief Sets the id of this record. The id field is getTableDefinition()->getFldIDName
 *
 * If the value is changed, sets the read flag to false
 * @param recid ...
 * @return void
 **/
void dbRecord::setRecordID(dbRecordID recid)
{
    Xtring idfldname( getTableDefinition()->getFldIDName() );
    if( idfldname.isEmpty() )
        idfldname = "ID";
    if( mFieldValues.find(idfldname) != mFieldValues.end() ) {
        if( mFieldValues[idfldname].toUInt() != recid ) {
            mFieldValues.at(idfldname).setValue( recid );
            mIsRead = false;
        }
        if( recid != 0 )
            mIsNew = mIsDeleted = false;
    }
}

bool dbRecord::isEmpty( const Xtring &nocheckfields ) const
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        const_cast<dbRecord *>(this)->read( getRecordID() );
    XtringList nocheck_list;
    nocheckfields.tokenize( nocheck_list, ",");
    for ( unsigned int i = 0; i<pTableDef->getFieldCount(); i++ ) {
        dbFieldDefinition *flddef = pTableDef->getFieldDefinition(i);
        if( flddef->getName() == pTableDef->getFldIDName() )
            continue;
        if( nocheck_list.contains( flddef->getName() ) )
            continue;
        if( mFieldValues.seq_at(i).isEmpty() )
            continue;
        if( mFieldValues.seq_at(i).isNull() )
            continue;
        if( mFieldValues.seq_at(i) == flddef->customDefaultValue() )
            continue;
        return false;
    }
    return true;
}

void dbRecord::setModified ( bool changed )
{
    _GONG_DEBUG_PRINT(10, Xtring("set modified = ") + (changed ? "true" : "false") + " on " + getTableDefinition()->getName() );
    for ( unsigned int i = 0; i<pTableDef->getFieldCount(); i++ )
        mFieldValues.seq_at(i).setModified ( changed );
}

bool dbRecord::isModified() const
{
    for ( unsigned int i = 0; i<pTableDef->getFieldCount(); i++ ) {
        if ( mFieldValues.seq_at(i).isModified() )
            return true;
    }
    return false;
}

/*
    We could create a copy constructor for this, but then, we should have to reimplement
    the copy constructor of each class derived from dbRecord.
    If this were the only copy constructor and we use it in a derived class,
    the compiler generates a copy constructor for the derived classes and so calls first the
    copy constructor of dbRecord and then the default copy constructors for the derived classes
    so it is not what we might want
*/

bool dbRecord::copyRecord( dbRecord *other, bool deep,
                           const Xtring &inc_fields, const Xtring &noinc_fields)
{
    _GONG_DEBUG_ASSERT ( other );
    if( this == other )
        return true;
//     _GONG_DEBUG_PRINT(10, Xtring::printf("Table: %s, this=%d, other=%d",
//                                         getTableName().c_str(), getRecordID(), other->getRecordID() ) );
    XtringList noinc_fields_list, inc_fields_list;
    inc_fields.tokenize( inc_fields_list, ",");
    noinc_fields.tokenize( noinc_fields_list, ",");
    if( deep )
        clear( false );
    if( getTableName() == other->getTableName() )
    {
        /* If the records are from the same table, copy everything */
        mFilters = other->mFilters;
        mIsNew = other->mIsNew;
        mIsDeleted = other->mIsDeleted;
        mIsRead = true; // Avoid reading this record
        mErrors = other->mErrors;
        pUser = other->pUser;
        if( deep ) {
            clearRelations();
            for( uint i=0; i< mRecordRelations.size(); ++i ) {
                dbRecordRelation *recrel = mRecordRelations.seq_at(i);
                dbRecordRelation *otherrel = other->mRecordRelations.seq_at(i);
//                 _GONG_DEBUG_PRINT(10, "Copying relation " + recrel->getRelationDefinition()->getFullName() );
                if( recrel->getType() == dbRelationDefinition::one2one ) {
                    // Don't copy this relation unless it has been read in the other record
                    dbRecord *r = otherrel->getRelatedRecord();
                    if( r && r->isRead() )
                        recrel->getRelatedRecord()->copyRecord(r, deep, inc_fields, noinc_fields);
                } else if( recrel->getType() == dbRelationDefinition::aggregate ) {
                    if( otherrel->isRelatedRecordCreated() ) {
                        dbRecord *r = otherrel->getRelatedRecord();
                        recrel->getRelatedRecord()->copyRecord(r, deep, inc_fields, noinc_fields);
                    }
                } else if ( recrel->getType() == dbRelationDefinition::one2many
                            || recrel->getType() == dbRelationDefinition::many2many ) {
                    int nr;
                    for( nr = 0; nr < (int)otherrel->getRelatedRecordList()->size(); nr ++ ) {
                        if( otherrel->getRelatedRecord(nr) ) {
                            dbRecord *r = otherrel->getRelatedRecord(nr)->duplicate();
                            r->copyRecord( otherrel->getRelatedRecord(nr), deep, inc_fields, noinc_fields );
                            recrel->addRelatedRecord(r);
                        }
                    }
                    // Copy the original details so that saving the details works properly
                    for( nr = 0; nr < (int)otherrel->getRelatedRecordListOrig()->size(); nr ++ ) {
                        dbRecord *relorigrecord = otherrel->getRelatedRecordListOrig()->getRecord(nr);
                        if( relorigrecord ) {
                            dbRecord *r = relorigrecord->duplicate();
                            r->copyRecord( relorigrecord, deep, inc_fields, noinc_fields );
                            recrel->getRelatedRecordListOrig()->addRecord(r);
                        }
                    }
                }
            }
        }
        for ( unsigned int nf=0; nf< getFieldCount(); nf++ )
        {
            Xtring fldname = getTableDefinition()->getFieldDefinition(nf)->getName();
            if( inc_fields_list.size() && !inc_fields_list.contains( fldname  ) )
                continue;
            if( noinc_fields_list.contains( fldname ) )
                continue;
            if ( other->isNullValue(nf) )
                setNullValue(nf);
            else
                setValue( nf, other->getValue(nf) );
        }
        setModified( false );
        return true;
    } else {
        /* Try to copy all the matching fields */
        for ( dbFieldDefinitionDict::const_iterator it = pTableDef->getFieldDefinitions().begin();
                it != pTableDef->getFieldDefinitions().end();
                ++ it ) {
            Xtring fldname = it->second->getName();
            if( !other->getTableDefinition()->findFieldDefinition( fldname, false ) )
                continue; // The other does not have this field
            if( inc_fields_list.size() && !inc_fields_list.contains( fldname ) )
                continue;
            if( noinc_fields_list.contains( fldname ) )
                continue;
            if( fldname == getTableDefinition()->getFldIDName() )
                continue;
            _GONG_DEBUG_PRINT(4, "Copying field " + fldname + ", value=" + other->getValue( fldname ).toString() );
            if ( other->isNullValue(fldname) )
                setNullValue(fldname);
            else
                setValue( fldname, other->getValue( fldname ) );
        }
        return false;
    }
}

void dbRecord::clear( bool setcustomvalues )
{
    for ( unsigned int i = 0; i<pTableDef->getFieldCount(); i++ ) {
		const dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( i );
		if( flddef->getName() == pTableDef->getFldIDName() ) {
			// To avoid reading related records
			mFieldValues.seq_at(i).clear();
			break;
		}
	}
	// Clear relations first and then set default values of reference ids
    clearRelations(); // Do not set customvalues on related records
    for ( unsigned int i = 0; i<pTableDef->getFieldCount(); i++ ) {
		const dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( i );
        if( !setcustomvalues ) {
			if( flddef->isReference() ) {
				setValue(i, 0);
			} else {
				mFieldValues.seq_at(i).clear();
			}
			mOrigFieldValues.seq_at(i).clear();
        } else {
			Variant customvalue = flddef->customDefaultValue();
/*
			_GONG_DEBUG_PRINT(0, Xtring::printf("Set custom default value for '%s'(%s) to '%s'(%s)",
  								flddef->getFullName().c_str(),
  								Variant::typeToName(mFieldValues.seq_at(i).toVariant().type() ),
  								customvalue.toString().c_str(),
  								flddef->getDefaultValue().c_str() ) );
*/
            switch ( flddef->getSqlColumnType() ) {
            case SQLINTEGER:
				// If this field is a reference, setting its default value must set the id of the related record, which setValue does
				if( customvalue.toInt() && flddef->isReference() ) {
					setValue(i, customvalue.toInt() );
					mOrigFieldValues.seq_at(i).clear( customvalue.toInt() );
				} else {
					mFieldValues.seq_at(i).clear( customvalue.toInt() );
					mOrigFieldValues.seq_at(i).clear( customvalue.toInt() );
				}
                break;
            case SQLSTRING:
            case SQLBLOB:
            case SQLTEXT:
                mFieldValues.seq_at(i).clear ( customvalue.toString() );
                mOrigFieldValues.seq_at(i).clear ( customvalue.toString() );
                break;
            case SQLDATE:
                mFieldValues.seq_at(i).clear ( customvalue.toString() );
                mOrigFieldValues.seq_at(i).clear ( customvalue.toString() );
                break;
            case SQLTIME:
                mFieldValues.seq_at(i).clear ( customvalue.toString() );
                mOrigFieldValues.seq_at(i).clear ( customvalue.toString() );
                break;
            case SQLDATETIME:
            case SQLTIMESTAMP:
                mFieldValues.seq_at(i).clear ( customvalue.toDateTime() );
                mOrigFieldValues.seq_at(i).clear ( customvalue.toDateTime() );
                break;
            case SQLDECIMAL:
                mFieldValues.seq_at(i).clear ( Money ( customvalue.toString().toDoubleLocIndep(), flddef->getDecimals() ) );
                mOrigFieldValues.seq_at(i).clear ( Money ( customvalue.toString().toDoubleLocIndep(), flddef->getDecimals() ) );
                break;
            case SQLFLOAT:
                mFieldValues.seq_at(i).clear ( customvalue.toString().toDoubleLocIndep() );
                mOrigFieldValues.seq_at(i).clear ( customvalue.toString().toDoubleLocIndep() );
                break;
            case SQLBOOL:
                mFieldValues.seq_at(i).clear ( customvalue.toBool() );
                mOrigFieldValues.seq_at(i).clear ( customvalue.toBool() );
                break;
            }
        }
    }
    setModified ( false );
    for( dbRecordBehaviorsList::const_iterator bit = getTableDefinition()->getRecordBehaviors().begin();
            bit != getTableDefinition()->getRecordBehaviors().end(); ++ bit ) {
        (*bit)->clear( this, setcustomvalues );
    }
}

void dbRecord::setNew( bool isnew )
{
    mIsDeleted = false;
    if ( isnew != mIsNew ) {
        mIsNew = isnew;
        setModified ( true );
    }
    if ( mIsNew ) {
        setRecordID( 0 );
        // set new the aggregate relations
        for( dbRecordRelationDict::const_iterator relit = mRecordRelations.begin();
                relit != mRecordRelations.end(); ++relit ) {
            dbRecordRelation *recrel = relit->second;
            if ( recrel->isEnabled() )
            {
                if ( recrel->getType() == dbRelationDefinition::aggregate ) {
                    recrel->getRelatedRecord(-1)->setNew( isnew );
                    if( isnew )
                        setValue( recrel->getLeftField(), 0 );
                }
            }
        }
    }
}

void dbRecord::clearRelations()
{
    for( dbRecordRelationDict::const_iterator relit = mRecordRelations.begin();
            relit != mRecordRelations.end(); ++relit ) {
        dbRecordRelation *recrel = relit->second;
        if ( recrel->isEnabled() ) {
            if ( recrel->getType() == dbRelationDefinition::one2one
                    || recrel->getType() == dbRelationDefinition::aggregate ) {
                // Test if the related record has not been created
                if( recrel->pRelatedRecord /* No uso getRelatedRecord porque entonces lo crea */ )
                    recrel->getRelatedRecord(-1)->clear( false );
                setValue( recrel->getLeftField(), 0 );
            } else if ( recrel->getType() == dbRelationDefinition::one2many
                        || recrel->getType() == dbRelationDefinition::many2many) {
                recrel->getRelatedRecordList()->clear();
                recrel->getRelatedRecordListOrig()->clear();
            }
        }
    }
}

bool dbRecord::SELECT ( const Xtring &where )
{
    _GONG_DEBUG_ASSERT ( pConn );
    mIsRead = true; // Avoid recursion in setValue
    Xtring select = "SELECT ";
    for ( unsigned int i=0; i<getFieldCount(); i++ )
    {
        if ( i!=0 )
            select += ",";
        select += pConn->nameToSQL ( pTableDef->getFieldDefinition ( i )->getName() );
    }
    select += " FROM " + pConn->nameToSQL ( getTableName() ) + " WHERE " + where;
    std::auto_ptr<dbResultSet> rs ( pConn->select ( select ) );
    if ( rs->next() ) {
        for ( unsigned int i=0; i<getFieldCount(); i++ ) {
            dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( i );
            switch ( flddef->getSqlColumnType() ) {
            case SQLINTEGER:
            case SQLSTRING:
            case SQLTEXT:
            case SQLDATE:
            case SQLTIME:
            case SQLDATETIME:
            case SQLTIMESTAMP:
            case SQLBOOL:
            case SQLFLOAT:
				{
					Variant value( rs->getValue(i) );
					setValue( i, value );
				}
				break;
            case SQLBLOB:
				setValue( i, rs->toBinary(i) );
                break;
            case SQLDECIMAL:
                setValue( i, Money( rs->toDouble(i),
                                    pTableDef->getFieldDefinition(i)->getDecimals() ) );
                break;
            }
//             _GONG_DEBUG_PRINT(3, Xtring::printf("Setting origvalue[%d](%s)=%s(%s)",
// 												i, Variant::typeToName( mOrigFieldValues.seq_at(i).type() ),
// 												value.toString().c_str(), Variant::typeToName( value.type() ) ) );
            mOrigFieldValues.seq_at(i).setValue(mFieldValues.seq_at(i));
            if ( rs->isNull ( i ) ) {
                setNullValue ( i );
                mOrigFieldValues.seq_at(i).setNull();
            }
        }
        setNew ( false );
        setModified ( false );
        return true;
    } else {
        mIsRead = false;
    }
    return false;
}

bool dbRecord::INSERT()
{
    _GONG_DEBUG_ASSERT ( pConn );
    Xtring fields, values;
    int ret = true;
    for ( unsigned int i=0; i<getFieldCount(); i++ )
    {
        const dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( i );
        Xtring fldname = flddef->getName();
        dbFieldValue &fldval = mFieldValues.seq_at(i);
        fields += "," + pConn->nameToSQL( fldname );
        if ( (flddef->canBeNull() || flddef->isPrimaryKey() )
                && ( fldval.isNull() || fldval.isEmpty() ) )
            values += ",NULL";
        else
            values += "," + flddef->toSQL ( pConn, fldval, true /*inserting*/ );
    }
    if ( !values.isEmpty() ) {
        fields[0]=' ';
        values[0]=' ';
        ret = pConn->exec ( "INSERT INTO " + pConn->nameToSQL ( getTableName() ) + "(" + fields + ")VALUES(" + values + ")" );
        if ( ret ) {
            setRecordID ( pConn->getLastInsertID ( getTableName(), getTableDefinition()->getFldIDName() ) );
            setNew ( false );
            setModified ( false ); // after setNew, because setNew calls setModified(true)
            mIsRead = true;
            mIsDeleted = false;
        }
    }
    return ret;
}

bool dbRecord::UPDATE()
{
    _GONG_DEBUG_ASSERT ( pConn );
    Xtring update;
    int ret = true;
    for ( unsigned int i=0; i<getFieldCount(); i++ )
    {
        const dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( i );
        Xtring fldname = flddef->getName();
        const dbFieldValue &fldval = mFieldValues.seq_at(i);
        if ( fldval.isModified() )
        {
            if ( flddef->canBeNull() && ( fldval.isNull() || fldval.isEmpty() ) )
                update += "," + fldname + "= NULL";
            else
                update += "," + fldname + "=" + flddef->toSQL ( pConn, fldval, false /* updating */ );
        }
    }
    if ( !update.isEmpty() )
    {
        update[0]=' ';
        ret = pConn->exec ( "UPDATE " + pConn->nameToSQL ( getTableName() )
                            + " SET " + update
                            + " WHERE " + getTableDefinition()->getFldIDName() + "=" + pConn->toSQL ( getRecordID() ) );
        if ( ret )
            setModified ( false );
    } else {
        _GONG_DEBUG_WARNING("Updating " + getTableDefinition()->getName() + " with not modified values" );
    }
    return ret;
}

bool dbRecord::DELETE()
{
    _GONG_DEBUG_ASSERT ( pConn );
    int ret = false;
    if ( !isNew() )
    {
        Xtring cond;
        if( getTableDefinition()->getFldIDName().isEmpty() ) {
            // Delete using all the field values;
            for ( unsigned int i=0; i<getFieldCount(); i++ ) {
                const dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( i );
                Xtring fldname = flddef->getName();
                const dbFieldValue &fldval = mFieldValues.seq_at(i);
                if( !cond.isEmpty() )
                    cond += " AND ";
                cond += pConn->nameToSQL(fldname) + "=" + flddef->toSQL ( pConn, fldval );
            }
        } else {
            if ( getRecordID() == 0 ) {
                _GONG_DEBUG_WARNING ( getTableName() + ".ID=0" );
            }
            cond += pConn->nameToSQL(getTableDefinition()->getFldIDName()) + "=" + pConn->toSQL ( getRecordID() );
        }
        ret = pConn->exec ( "DELETE FROM " + pConn->nameToSQL ( getTableName() ) + " WHERE " + cond );
        if ( ret ) {
            mIsRead = false;
            mIsDeleted = true;
        }
    }
    return ret;
}

bool dbRecord::hasEnabledRelations() const
{
    for( dbRecordRelationDict::const_iterator relit = mRecordRelations.begin();
            relit != mRecordRelations.end(); ++relit ) {
        dbRecordRelation *recrel = relit->second;
        if ( recrel->isEnabled() )
            return true;
    }
    return false;
}


bool dbRecord::read( const Xtring &sql )
{
    _GONG_DEBUG_ASSERT ( pConn );
    for( dbRecordBehaviorsList::const_iterator bit = getTableDefinition()->getRecordBehaviors().begin();
            bit != getTableDefinition()->getRecordBehaviors().end(); ++ bit ) {
        if( !(*bit)->read( this, BEHAVIOR_PRE, sql ) )
            return false;
    }
	mErrors.clear();
    bool ret = SELECT( sql );
    if ( !ret )	{
        setNew( true );
        clear( true );
        clearRelations();
        setModified(false);
    } else {
        readRelated( false );
    }
    for( dbRecordBehaviorsList::const_iterator bit = getTableDefinition()->getRecordBehaviors().begin();
            bit != getTableDefinition()->getRecordBehaviors().end(); ++ bit ) {
        if( !(*bit)->read( this, BEHAVIOR_POST, sql ) )
            return false;
    }
    return ret;
}

bool dbRecord::read(dbRecordID recid)
{
    if( recid == 0 ) {
        mIsRead = true; // Avoid recursion
        setNew( true );
        clear( true );
        clearRelations();
        setModified( false );
		mErrors.clear();
        return false;
    } else {
        return read(getTableDefinition()->getFldIDName() + "=" + pConn->toSQL(recid));
    }
}


bool dbRecord::readWithFilter(const Xtring& where)
{
    Xtring wholewhere = getFilter( "", where );
    return read( wholewhere );
}


bool dbRecord::readRelated( bool force )
{
    _GONG_DEBUG_ASSERT ( pConn );
    for( dbRecordRelationDict::const_iterator relit = mRecordRelations.begin();
            relit != mRecordRelations.end(); ++relit ) {
        dbRecordRelation *recrel = relit->second;
        if ( recrel->isEnabled() )
        {
            Variant leftvalue = getValue ( recrel->getLeftField() );
            if ( recrel->getType() == dbRelationDefinition::one2one
                    || recrel->getType() == dbRelationDefinition::aggregate )
            {
                _GONG_DEBUG_PRINT ( 3, Xtring::printf ( "Reading 1:1 relationship: %s", recrel->getRelationDefinition()->getFullName().c_str() ) );
                if ( leftvalue.isValid() )
                {
                    dbRecord *r = recrel->getRelatedRecord(-1);
                    r->setRecordID( leftvalue.toInt() );
                    if( leftvalue.toInt() == 0 ) {
                        r->clear( false );
                        r->setNew();
                    }
                }
            } else if ( recrel->getType() == dbRelationDefinition::one2many
                        ||  recrel->getType() == dbRelationDefinition::many2many ) {
                _GONG_DEBUG_PRINT( 3, Xtring::printf ( "Reading 1:M or M:M relationship: %s", recrel->getRelationDefinition()->getFullName().c_str() ) );
                recrel->getRelatedRecordList()->clear();
                recrel->getRelatedRecordListOrig()->clear();
                if ( leftvalue.isValid() && !leftvalue.toInt() == 0 )
                {
                    std::auto_ptr<dbResultSet> rs ( pConn->select ( "SELECT * FROM "
                                                    + pConn->nameToSQL ( recrel->getRightTable() )
                                                    + " WHERE " + pConn->nameToSQL(recrel->getRightTable())
                                                    + "." + pConn->nameToSQL(recrel->getRightField())
                                                    + " = " + pConn->toSQL ( leftvalue )
                                                    + " ORDER BY 1" ) );
                    while( rs->next() ) {
                        dbRecord *relatedrecord = recrel->getRelatedRecord(-1 );
                        dbRecord *newrelrecord = relatedrecord->duplicate();
                        newrelrecord->setValuesFromRow( &*rs, rs->getRowNumber() );
                        newrelrecord->setNew( false );
                        recrel->getRelatedRecordList()->addRecord( newrelrecord );
                        dbRecord *origrecord = newrelrecord->duplicate();
                        origrecord->setValuesFromRow( &*rs, rs->getRowNumber() );
                        origrecord->setNew( false );
                        recrel->getRelatedRecordListOrig()->addRecord( origrecord );
                    }
                }
            }
        }
    }
    return true;
}

bool dbRecord::save( bool saverelated )
{
    _GONG_DEBUG_ASSERT ( pConn );
    _GONG_DEBUG_PRINT(3, "Saving table: " + getTableName() );
    bool ret = true;
    try
    {
        if ( hasEnabledRelations() )
            pConn->beginTransaction();
        for( dbRecordBehaviorsList::const_iterator bit = getTableDefinition()->getRecordBehaviors().begin();
                bit != getTableDefinition()->getRecordBehaviors().end();
                ++ bit ) {
            if( !(*bit)->beforeSave( this, ret ) ) {
                pConn->rollbackTransaction();
                return ret;
            }
        }
        // Save the record to the database
        if( (isNew() && getRecordID() != 0 ) || (!isNew() && getRecordID() == 0 ) ) {
            _GONG_DEBUG_WARNING(Xtring::printf("The record from table %s either is new and has id !=0 or is not new and has id == 0",
                                               getTableDefinition()->getName().c_str()));
        }
        if ( isNew() )
        {
            ret = INSERT();
            if ( ret && saverelated )
                ret = saveRelated( false );
        }
        else
        {
            if( saverelated )
                ret = removeRelated( true );
            if ( ret )
                ret = UPDATE();
            if ( ret && saverelated )
                ret = saveRelated( true );
        }
        if( ret ) {
			mErrors.clear();
            for( dbRecordBehaviorsList::const_iterator bit = getTableDefinition()->getRecordBehaviors().begin();
                    bit != getTableDefinition()->getRecordBehaviors().end();
                    ++ bit ) {
                if( !(*bit)->afterSave( this, ret ) ) {
                    ret = false;
                    break;
                }
            }
        }
        if ( hasEnabledRelations() ) {
            if ( ret )
                pConn->commitTransaction();
            else
                pConn->rollbackTransaction();
        }
    }
    catch ( ... )
    {
        if ( hasEnabledRelations() )
            pConn->rollbackTransaction();
        throw;
    }
    return ret;
}


/**
 * @brief saves the related records
 *
 * @param updating if updating, the aggregate records have not been deleted, so they are just overwritten
 * @return bool
 **/
bool dbRecord::saveRelated( bool updating )
{
    for( dbRecordRelationDict::const_iterator relit = mRecordRelations.begin();
            relit != mRecordRelations.end(); ++relit ) {
        dbRecordRelation *recrel = relit->second;
        if ( recrel->isEnabled() )
        {
            // one2one relations are independent, so they are not saved here
            if ( recrel->getType() == dbRelationDefinition::aggregate )
            {
                _GONG_DEBUG_PRINT(4, Xtring::printf("Saving aggregate relation %s",
                                                    recrel->getRelationDefinition()->getFullName().c_str() ) );
                Variant leftvalue = getValue( recrel->getLeftField() );
                Variant rightvalue;
                dbRecord *agg_record = recrel->getRelatedRecord(-1 );
                if( !leftvalue.isEmpty() && agg_record->isEmpty() ) { // Remove related record
                    rightvalue = 0;
                    agg_record->beforeDeleteRelated( this );
                    agg_record->remove();
                } else {
                    agg_record->beforeSaveRelated( this );
                    agg_record->save( true );
                    rightvalue = agg_record->getValue( recrel->getRightField() );
                }
                // Update this record reference to the aggregated one
                if( rightvalue != leftvalue ) {
                    setValue( recrel->getLeftField(), rightvalue );
                    save( false ); // Dont save related again
                }
            } else if ( recrel->getType() == dbRelationDefinition::one2many
                        || recrel->getType() == dbRelationDefinition::many2many ) {
                _GONG_DEBUG_PRINT(4, Xtring::printf("Saving 1:M or M:M relation %s",
                                                    recrel->getRelationDefinition()->getFullName().c_str() ) );
                Variant leftvalue = getValue ( recrel->getLeftField() );
			// Como da muchos errores la optimización, la elimino.
				bool optimizing_out = false; // updating;
                for( uint nr = 0; nr < recrel->getRelatedRecordList()->size(); nr ++ ) {
					dbRecord *detail = recrel->getRelatedRecordList()->at(nr);
                    // Detect as much identical records as we can
                    if( optimizing_out ) {
						if( nr < recrel->getRelatedRecordListOrig()->size() ) {
							dbRecord *related_record_orig = recrel->getRelatedRecordListOrig()->at(nr);
							if( detail->toString( TOSTRING_DEBUG_COMPLETE )
                                != related_record_orig->toString( TOSTRING_DEBUG_COMPLETE ) )
								optimizing_out = false;
						} else {
							optimizing_out = false;
						}
                    }
                    if( !optimizing_out ) {
                        if ( ! ( detail->isEmpty() ) ) {
                            // If we are updating, all the records must be new now, as they have been deleted before
                            detail->setNew( true );
                            detail->setValue( recrel->getRightField(), leftvalue );
                            detail->beforeSaveRelated( this );
                            detail->save( true );
                        }
                    } else {
                        _GONG_DEBUG_PRINT(1, "Record " + Xtring::number(nr) + " has been optimized");
                    }
                }
            }
        }
    }
    return true;
}

bool dbRecord::remove()
{
    _GONG_DEBUG_ASSERT ( pConn );
    bool ret = false;
    try {
        if ( hasEnabledRelations() )
            pConn->beginTransaction();
        for( dbRecordBehaviorsList::const_iterator bit = getTableDefinition()->getRecordBehaviors().begin();
                bit != getTableDefinition()->getRecordBehaviors().end();
                ++ bit ) {
            if( !(*bit)->remove( this, BEHAVIOR_PRE ) ) {
                pConn->rollbackTransaction();
                return false;
            }
        }
        ret = removeRelated( false );
        if ( ret ) // Remove this record
            ret = DELETE();
        if( ret ) {
			mErrors.clear();
            for( dbRecordBehaviorsList::const_iterator bit = getTableDefinition()->getRecordBehaviors().begin();
                    bit != getTableDefinition()->getRecordBehaviors().end();
                    ++ bit ) {
                if( !(*bit)->remove( this, BEHAVIOR_POST ) ) {
                    ret = false;
                    break;
                }
            }
        }
        if ( hasEnabledRelations() ) {
            if ( ret )
                pConn->commitTransaction();
            else
                pConn->rollbackTransaction();
        }
    }
    catch ( ... ) {
        if ( hasEnabledRelations() )
            pConn->rollbackTransaction();
        throw;
    }
    return ret;
}

/**
 * @brief Removes the related records.
 *
 * @param updating If true, the master record is being updated, if false, it is being deleted
 * @return bool
 **/
bool dbRecord::removeRelated( bool updating )
{
    _GONG_DEBUG_ASSERT ( pConn );
    for( dbRecordRelationDict::const_iterator relit = mRecordRelations.begin();
            relit != mRecordRelations.end(); ++relit ) {
        dbRecordRelation *recrel = relit->second;
        if ( recrel->isEnabled() )
        {
            if ( recrel->getType() == dbRelationDefinition::aggregate ) {
                Variant leftvalue = getValue( recrel->getLeftField() );
                if ( leftvalue.isValid()) {
                    dbRecord *agg_record = recrel->getRelatedRecord(-1 );
                    if( agg_record && !agg_record->isNew() ) {
                        agg_record->beforeDeleteRelated( const_cast<dbRecord *>(this) );
                        if( !updating )
                            agg_record->remove();
                    }
                    if( !updating )
                        const_cast<dbRecord *>(this)->setValue( recrel->getLeftField(), 0 );
                }
            } else if ( recrel->getType() == dbRelationDefinition::one2many
                        || recrel->getType() == dbRelationDefinition::many2many )
            {
                _GONG_DEBUG_PRINT(3, Xtring::printf("Removing 1:M or M:M relation %s",
                                                    recrel->getRelationDefinition()->getFullName().c_str() ) );
                Variant leftvalue = getValue ( recrel->getLeftField() );
                if ( leftvalue.isValid() )
                {
                    // Detect as much identical records as we can
                    bool optimizing_out = false; // updating;
                    for( uint nr = 0; nr < recrel->getRelatedRecordListOrig()->size(); nr ++ ) {
                        dbRecord *related_record_orig = recrel->getRelatedRecordListOrig()->at(nr);
						if( optimizing_out ) {
							if( nr < recrel->getRelatedRecordList()->size() ) {
								if( recrel->getRelatedRecordList()->at(nr)->toString( TOSTRING_DEBUG_COMPLETE )
									!= related_record_orig->toString( TOSTRING_DEBUG_COMPLETE ) )
									optimizing_out = false;
							} else {
								optimizing_out = false;
							}
						}
                        if( !optimizing_out ) {
                            if( related_record_orig->getRecordID() ) {
                                related_record_orig->beforeDeleteRelated( const_cast<dbRecord *>(this) );
                                related_record_orig->remove();
                            } else {
                                _GONG_DEBUG_WARNING("Not removing record because it is new: "
                                                    + recrel->getRelatedRecord(nr)->toString( TOSTRING_DEBUG_COMPLETE ) );
                            }
                        } else {
                            _GONG_DEBUG_PRINT(1, "Record " + Xtring::number(nr) + " has been optimized");
                        }
                    }
#ifdef _GONG_DEBUG
                    if( !updating ) {
                        // Then, delete all related records, even if they were not read
                        dbResultSet *rs = pConn->select ( "SELECT * FROM " + recrel->getRightTable()
                                                          + " WHERE " + recrel->getRightTable()
                                                          + "." + recrel->getRightField()
                                                          + " = " + pConn->toSQL ( leftvalue ) );
                        if( rs->next() ) {
                            _GONG_DEBUG_WARNING( "There were orphan details" );
                        }
                        delete rs;
                    }
#endif
                }
            }
        }
    }
    return true;
}

int dbRecord::addRelation ( dbRecordRelation *rel )
{
    mRecordRelations.insert ( rel->getName(), rel );
    return mRecordRelations.size();
}

int dbRecord::removeRelation ( unsigned int index )
{
    if ( index >=0 && index < mRecordRelations.size() )
    {
        // No borrar el registro, porque la relacion no lo crea.
        mRecordRelations.erase ( index );
    }
    return index;
}

void dbRecord::removeRelations()
{
    for ( unsigned int i=0; i<mRecordRelations.size(); i++ )
        removeRelation ( i );
}

int dbRecord::getRelationIndex( const Xtring &relname ) const
{
    // TODO: Refactor: use the dictionary key
    for ( unsigned int i=0; i<mRecordRelations.size(); i++ )
    {
        if( getRelation( i )->getName() == relname )
            return i;
        else if( getRelation( i )->getName() == getTableName() + "." + relname )
            return i;
    }
    for ( unsigned int i=0; i<mRecordRelations.size(); i++ ) {
		_GONG_DEBUG_PRINT(5, getRelation(i)->getRelationDefinition()->getFullName());
	}
    return -1;
}

dbRecordRelation *dbRecord::getRelation( unsigned int index ) const
{
    if ( index < 0 || index >= mRecordRelations.size() ) {
        return 0;
    } else {
        return mRecordRelations.seq_at(index);
    }
}

dbRecordRelation *dbRecord::findRelation ( const Xtring &relname ) const
{
    int i = getRelationIndex( relname );
    if ( i == -1 ) 	{
        return 0;
    } else {
        return getRelation ( i );
    }
}

/// \todo Tell between dbFieldReferenceID and dbFieldxxxxxxRelation
dbRecordRelation *dbRecord::findRelationByRelatedTable ( const Xtring & tablename ) const
{
    for ( unsigned int i=0; i<mRecordRelations.size(); i++ ) {
        if ( getRelation ( i )->getRightTable() == tablename ) {
            return getRelation ( i );
        }
    }
#ifdef _GONG_DEBUG
    _GONG_DEBUG_WARNING ( Xtring::printf ( "Table '%s' is not related with table '%s'",
                                           getTableName().c_str(), tablename.c_str() ) );
    if ( ::__gong_debug::_gong_debug_level > 2 )
        std::cout << *getTableDefinition() << std::endl;
#endif
    return 0;
}

dbRecordRelationDict dbRecord::findRelationsBySemanticProperties(const XtringList& properties) const
{
    dbRecordRelationDict ret;
    for( unsigned int i=0; i<mRecordRelations.size(); i++ ) {
        dbRecordRelation *rel = getRelation(i);
        for( XtringList::const_iterator pit = properties.begin();
                pit != properties.end(); ++pit ) {
            if( rel->getRelatedRecord()->getSemanticProperties().contains( *pit ) ) {
                ret.insert( rel->getName(), rel );
            }
        }
    }
    return ret;
}

dbRecord *dbRecord::getRelatedRecord( unsigned int index ) const
{
    dbRecordRelation *recrel = getRelation( index );
    if ( recrel )
        return recrel->getRelatedRecord(-1 );
    else
        return 0;
}

dbRecord *dbRecord::findRelatedRecord ( const Xtring &relname ) const
{
    int relindex = getRelationIndex( relname );
    if( relindex == -1 ) {
        _GONG_DEBUG_WARNING( "Relation " + relname + " not found for record " + getTableName() );
        throw std::runtime_error( "Relation " + relname + " not found for record " + getTableName() );
    }
    return getRelatedRecord ( relindex );
}


bool dbRecord::addRelatedDetailRecord ( const Xtring &reltable, const dbRecord *relrecord )
{
    for ( unsigned int i=0; i<mRecordRelations.size(); i++ )
    {
        if ( getRelation(i)->getRightTable().upper() == reltable.upper()
                && getRelation(i)->getRelationDefinition()->getType() == dbRelationDefinition::one2many )
        {
            getRelation ( i )->getRelatedRecordList()->addRecord ( relrecord );
            _GONG_DEBUG_PRINT ( 6, relrecord->toString ( TOSTRING_DEBUG_COMPLETE ) );
            return true;
        }
    }
    _GONG_DEBUG_WARNING ( Xtring::printf ( "Table %s is not one2many-related to table %s",
                                           reltable.c_str(), getTableName().c_str() ) );
    return false;
}


dbFieldValue dbRecord::getValue( unsigned int nfield ) const
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        const_cast<dbRecord *>(this)->read( getRecordID() );
    if ( nfield < mFieldValues.size() ) {
        return mFieldValues.seq_at(nfield);
    } else {
        _GONG_DEBUG_WARNING ( Xtring::printf ( "Field number '%d' not found in table '%s'",
                                               nfield, getTableName().c_str() ) );
        return Variant();
    }
}

dbFieldValue dbRecord::getOrigValue(unsigned int nfield) const
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        const_cast<dbRecord *>(this)->read( getRecordID() );
    if ( nfield < mOrigFieldValues.size() ) {
        return mOrigFieldValues.seq_at(nfield);
    } else {
        _GONG_DEBUG_WARNING ( Xtring::printf ( "Field number '%d' not found in table '%s'",
                                               nfield, getTableName().c_str() ) );
        return Variant();
    }
}

bool dbRecord::isNullValue ( unsigned int nfield ) const
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        const_cast<dbRecord *>(this)->read( getRecordID() );
    if ( nfield < mFieldValues.size() ) {
        return mFieldValues.seq_at(nfield).isNull();
    } else {
        _GONG_DEBUG_WARNING ( Xtring::printf ( "Field number '%d' not found in table '%s'",
                                               nfield, getTableName().c_str() ) );
        return false;
    }
}

bool dbRecord::isNullOrigValue(unsigned int nfield) const
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        const_cast<dbRecord *>(this)->read( getRecordID() );
    if ( nfield < mOrigFieldValues.size() ) {
        return mOrigFieldValues.seq_at(nfield).isNull();
    } else {
        _GONG_DEBUG_WARNING ( Xtring::printf ( "Field number '%d' not found in table '%s'",
                                               nfield, getTableName().c_str() ) );
        return false;
    }
}

bool dbRecord::isNullValue ( const Xtring &fullfldname ) const
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        const_cast<dbRecord *>(this)->read( getRecordID() );
    Xtring tablename = dbFieldDefinition::extractTableName ( fullfldname );
    Xtring fldname = dbFieldDefinition::extractFieldName ( fullfldname );
    if ( tablename.isEmpty() || tablename == getTableName() ) {
        const dbFieldDefinition *flddef = findFieldDefinition( fldname );
        if( flddef && flddef->isCalculated() ) {
            return isEmpty();
        } else if ( mFieldValues.find ( fldname ) != mFieldValues.end() ) {
            return mFieldValues[fldname].isNull();
        }
    } else {
        if ( dbRecordRelation *rel = findRelationByRelatedTable ( tablename ) ) {
            dbRecord *relatedr = rel->getRelatedRecord(-1);
            if( relatedr->isRead() ) {
                uint leftvalue = getValue( rel->getLeftField() ).toUInt();
                if( relatedr->getRecordID() != leftvalue ) {
                    _GONG_DEBUG_WARNING( Xtring::printf("%s.%s != %s.%s", tablename.c_str(), rel->getLeftField().c_str(), rel->getRightTable().c_str(), rel->getRightField().c_str() ) );
                    const_cast<dbRecord *>(this)->setValue( rel->getLeftField(), relatedr->getRecordID() );
                }
            }
            return relatedr->isNullValue( fldname );
        }
    }
    _GONG_DEBUG_WARNING ( Xtring::printf ( "Field '%s' not found in table %s",
                                           fullfldname.c_str(), getTableName().c_str() ) );
    return true;
}

bool dbRecord::isNullOrigValue(const Xtring& fullfldname) const
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        const_cast<dbRecord *>(this)->read( getRecordID() );
    Xtring tablename = dbFieldDefinition::extractTableName ( fullfldname );
    Xtring fldname = dbFieldDefinition::extractFieldName ( fullfldname );
    if ( tablename.isEmpty() || tablename == getTableName() ) {
        if ( mOrigFieldValues.find ( fldname ) != mOrigFieldValues.end() ) {
            return mOrigFieldValues[fldname].isNull();
        }
    } else {
        _GONG_DEBUG_WARNING("Testing null orig value of related record is not allowed");
    }
    _GONG_DEBUG_WARNING ( Xtring::printf ( "Field '%s' not found in table %s",
                                           fullfldname.c_str(), getTableName().c_str() ) );
    return true;

}

dbFieldValue dbRecord::getValue( const Xtring &fullfldname ) const
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        const_cast<dbRecord *>(this)->read( getRecordID() );
    Xtring tablename = dbFieldDefinition::extractTableName ( fullfldname );
    Xtring fldname = dbFieldDefinition::extractFieldName ( fullfldname );
    if ( tablename.isEmpty() || tablename == getTableName() ) {
        const dbFieldDefinition *flddef = findFieldDefinition( fldname );
        if( flddef && flddef->isCalculated() ) {
            return calcValue( fldname );
        } else if ( mFieldValues.find ( fldname ) != mFieldValues.end() ) {
            return mFieldValues[fldname];
        } else if (fldname == "~CODE_AND_DESC_WITH_TABLENAME") {
			return Variant(toString(TOSTRING_CODE_AND_DESC_WITH_TABLENAME));
        } else if (fldname == "~CODE_AND_DESC") {
			return Variant(toString(TOSTRING_CODE_AND_DESC));
        } else if (fldname == "~DESC") {
			return Variant(toString(TOSTRING_DESC));
        } else if (fldname == "~USER") {
			return Variant(toString(TOSTRING_USER));
		}
    } else {
        /* get a value from a related table
         * If the id of the related record doesn't match the reference id field in this record
         * it means that the related table has been read after this record.
         * The other possibility, that the reference field has changed whithout the id of the
         * related table being changed is not possible because the setValue function synchronizes
         * both values */
        if ( dbRecordRelation *rel = findRelationByRelatedTable ( tablename ) )	{
            dbRecord *relatedr = rel->getRelatedRecord(-1);
            if( relatedr->isRead() ) {
                uint leftvalue = getValue( rel->getLeftField() ).toUInt();
                if( relatedr->getRecordID() != leftvalue ) {
                    _GONG_DEBUG_WARNING( Xtring::printf("%s.%s != %s.%s", tablename.c_str(), rel->getLeftField().c_str(), rel->getRightTable().c_str(), rel->getRightField().c_str() ) );
                    const_cast<dbRecord *>(this)->setValue( rel->getLeftField(), relatedr->getRecordID() );
                }
            }
            return relatedr->getValue( fldname );
        }
    }
    _GONG_DEBUG_WARNING ( Xtring::printf ( "Field '%s' not found in table %s",
                                           fullfldname.c_str(), getTableName().c_str() ) );
    return Variant();
}

dbFieldValue dbRecord::getOrigValue(const Xtring& fullfldname) const
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        const_cast<dbRecord *>(this)->read( getRecordID() );
    Xtring tablename = dbFieldDefinition::extractTableName ( fullfldname );
    Xtring fldname = dbFieldDefinition::extractFieldName ( fullfldname );
    if ( tablename.isEmpty() || tablename == getTableName() ) {
        if ( mOrigFieldValues.find ( fldname ) != mOrigFieldValues.end() ) {
            return mOrigFieldValues[fldname];
        }
    } else {
        _GONG_DEBUG_WARNING("Getting orig value of related record is not allowed");
    }
    _GONG_DEBUG_WARNING ( Xtring::printf ( "Field '%s' not found in table %s",
                                           fullfldname.c_str(), getTableName().c_str() ) );
    return Variant();
}


dbFieldValue dbRecord::calcValue(const Xtring &fldname) const
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        const_cast<dbRecord *>(this)->read( getRecordID() );
    return Variant();
}

bool dbRecord::setNullValue ( unsigned int nfield )
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        read( getRecordID() );
    if ( nfield < mFieldValues.size() )
    {
        mFieldValues.seq_at(nfield).setNull();
        return true;
    }
    else
    {
        _GONG_DEBUG_WARNING ( Xtring::printf ( "Field number '%d' not found in table '%s'",
                                               nfield, getTableName().c_str() ) );
        return false;
    }
}

void dbRecord::setRelatedID( int nfield, const Variant &id )
{
    dbRecordRelation *relation = findRelation( getTableName() + "." + getTableDefinition()->getFieldDefinition(nfield)->getName() );
    if( relation && relation->isEnabled() ) {
		if( !relation->pRelatedRecord && id.toUInt() == 0)
			return; // No lo crees solo para ponerlo a cero
        dbRelationDefinition *reldef = relation->getRelationDefinition();
        if( reldef->getType() == dbRelationDefinition::one2one
                || reldef->getType() == dbRelationDefinition::aggregate ) {
            relation->getRelatedRecord(-1)->setRecordID( id.toUInt() );
        }
    }
}

bool dbRecord::setNullValue ( const Xtring &fullfldname )
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        read( getRecordID() );
    Xtring tablename = dbFieldDefinition::extractTableName ( fullfldname );
    Xtring fldname = dbFieldDefinition::extractFieldName ( fullfldname );
    if ( tablename.isEmpty() || tablename == getTableName() )
    {
        if ( mFieldValues.find ( fldname ) != mFieldValues.end() )
        {
            mFieldValues[fldname].setNull();
            setRelatedID( getTableDefinition()->getFieldPosition(fldname), Variant(0) );
            return true;
        }
    }
    else
    {
        if ( dbRecordRelation *rel = findRelationByRelatedTable ( tablename ) ) {
            dbRecord *relatedr = rel->getRelatedRecord(-1);
            if( relatedr->isRead() ) {
                uint leftvalue = getValue( rel->getLeftField() ).toUInt();
                if( relatedr->getRecordID() != leftvalue ) {
                    _GONG_DEBUG_WARNING( Xtring::printf("%s.%s != %s.%s", tablename.c_str(), rel->getLeftField().c_str(), rel->getRightTable().c_str(), rel->getRightField().c_str() ) );
                    const_cast<dbRecord *>(this)->setValue( rel->getLeftField(), relatedr->getRecordID() );
                }
            }
            return relatedr->setNullValue ( fldname );
        }
    }
    _GONG_DEBUG_WARNING ( Xtring::printf ( "Field '%s' not found in table %s",
                                           fullfldname.c_str(), getTableName().c_str() ) );
    return false;
}

bool dbRecord::setValue( unsigned int nfield, const Variant &value )
{
    if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        read( getRecordID() );
//   	_GONG_DEBUG_PRINT(3, Xtring::printf( "%s.%s(%d), value=%s", getTableName().c_str(),
// 										 getTableDefinition()->getFieldDefinition(nfield)->getName().c_str(),
// 										 nfield, value.toString().c_str() ) );
    if ( nfield < getFieldCount() )
    {
        bool wasnull = mFieldValues.seq_at(nfield).isNull();
        // If the field is null and the value is empty, do not change anything
        if( !(wasnull && value.isEmpty()) ) {
            mFieldValues.seq_at(nfield).setValue(value);
			const dbFieldDefinition *flddef = getTableDefinition()->getFieldDefinition ( nfield );
			if( flddef->isReference() ) {
                setRelatedID( nfield, value );
			}
            // If value is empty and the field was not null, set it to null if it can be null
            if ( value.isEmpty() && !wasnull )
            {
                if ( flddef->canBeNull() &&
                        ( flddef->isReference()
                          || flddef->getSqlColumnType() == SQLSTRING || flddef->getSqlColumnType() == SQLTEXT
                          || flddef->getSqlColumnType() == SQLDATE || flddef->getSqlColumnType() == SQLDATETIME
                          || flddef->getSqlColumnType() == SQLDATETIME || flddef->getSqlColumnType() == SQLTIMESTAMP ) )
                    mFieldValues.seq_at(nfield).setNull();
            }
        }
        return true;
    }
    else
    {
        _GONG_DEBUG_WARNING ( Xtring::printf ( "Field number '%d' not found in table '%s'",
                                               nfield, getTableName().c_str() ) );
        return false;
    }
}

bool dbRecord::setValue( const Xtring &fullfldname, const Variant &value )
{
	if( !mIsRead && !mIsDeleted && getRecordID() != 0 )
        read( getRecordID() );
//  	_GONG_DEBUG_PRINT(0, Xtring::printf ( "table=%s, fld=%s, value=%s", getTableName().c_str(), fullfldname.c_str(), value.toString().c_str() ) );
    Xtring tablename = dbFieldDefinition::extractTableName ( fullfldname );
    Xtring fldname = dbFieldDefinition::extractFieldName ( fullfldname );
    if( fldname.isEmpty() )
        return false;
    if ( tablename.isEmpty() || tablename == getTableName() )
    {
        if ( !isNew() && fldname.upper() == getTableDefinition()->getFldIDName() )
        {
            _GONG_DEBUG_WARNING ( getTableName() + ":Can't update ID value for non-new records" );
            return false;
        }
        Dictionary<dbFieldValue>::iterator it = mFieldValues.find(fldname);
        if( it != mFieldValues.end() )
        {
            bool wasnull = it->second.isNull();
            // If the field is null and the value is empty, do not change anything
            if( !(wasnull && value.isEmpty()) ) {
                it->second.setValue(value);
				const dbFieldDefinition *flddef = getTableDefinition()->findFieldDefinition( fldname );
				if( flddef ) {
					if( flddef->isReference() ) {
						setRelatedID( getTableDefinition()->getFieldPosition(fldname), value );
					}
					// If value is empty and the field was not null, set it to null if it can be null
					if ( value.isEmpty() && !wasnull ) {
						if ( flddef->canBeNull() &&
								( flddef->isReference()
									|| flddef->getSqlColumnType() == SQLSTRING || flddef->getSqlColumnType() == SQLTEXT
									|| flddef->getSqlColumnType() == SQLDATE || flddef->getSqlColumnType() == SQLDATETIME
									|| flddef->getSqlColumnType() == SQLDATETIME || flddef->getSqlColumnType() == SQLTIMESTAMP ) )
							it->second.setNull();
					}
				} else {
					_GONG_DEBUG_WARNING ( Xtring::printf ( "Field '%s' not found in table %s",
                                           fldname.c_str(), getTableName().c_str() ) );
				}
            }
            return true;
        }
    } else {
        if ( dbRecordRelation *rel = findRelationByRelatedTable( tablename ) ) {
            dbRecord *relatedr = rel->getRelatedRecord(-1);
            if( relatedr->isRead() ) {
                uint leftvalue = getValue( rel->getLeftField() ).toUInt();
                if( relatedr->getRecordID() != leftvalue ) {
                    _GONG_DEBUG_WARNING( Xtring::printf("%s.%s != %s.%s", tablename.c_str(), rel->getLeftField().c_str(), rel->getRightTable().c_str(), rel->getRightField().c_str() ) );
                    const_cast<dbRecord *>(this)->setValue( rel->getLeftField(), relatedr->getRecordID() );
                }
            }
            return relatedr->setValue( fldname, value );
        }
    }
    _GONG_DEBUG_WARNING ( Xtring::printf ( "Field '%s' not found in table %s",
                                           fullfldname.c_str(), getTableName().c_str() ) );
    return false;
}

bool dbRecord::setValuesFromRow( dbResultSet *rs, dbResultSet::size_type row )
{
    bool ret = false;
    mIsRead = true;
    mIsDeleted = false;
    for( unsigned int nf = 0; nf < rs->getColumnCount(); nf++ ) {
// 		_GONG_DEBUG_PRINT(10, Xtring::printf("%s=%s", rs->getFieldName(nf), rs->getRow(row).getValue(nf).toString().c_str() ) );
        ret |= setValue( rs->getColumnName(nf), rs->getValue(nf) );
    }
    return ret;
}

/****************************************************************
 *
 * FILTERS
 *
 ****************************************************************/

Xtring dbRecord::getFilter(const Xtring &where, const Xtring &addTo, const Xtring &condsSep) const
{
    Xtring ret;
    for ( XtringList::const_iterator it = mFilters.begin(); it!=mFilters.end(); ++ it ) {
        if ( it != mFilters.begin() )
            ret += condsSep;
        if( it->startsWith("(") )
            ret += *it;
        else
            ret += "(" + *it + ")";
    }
    _GONG_DEBUG_PRINT(4, "Filters del registro: " + ret );
    if( !addTo.isEmpty() && (!(mFilters.contains( addTo.trim() ) && condsSep == "AND")) ) {
        if( !ret.isEmpty() )
            ret += " " + condsSep + " ";
        if( addTo.startsWith("(") )
            ret += addTo;
        else
            ret += "(" + addTo + ")";
        _GONG_DEBUG_PRINT(4, "+ filtro adicional: " + ret );
    }
    if( !where.isEmpty() ) {
        if( !ret.isEmpty() )
            ret = " " + where + " " + ret;
        _GONG_DEBUG_PRINT(4, "+ filtro encadenado: " + ret );
    }

    return ret;
}

void dbRecord::addFilter ( const Xtring & filter )
{
    if ( !mFilters.contains ( filter.trim() ) )
        mFilters << filter.trim();
}

bool dbRecord::removeFilter ( const Xtring & filter )
{
    if ( filter.isEmpty() )
    {   // Deletes last added filter. To delete all of them, call removeFilters
        mFilters.erase ( mFilters.end() );
        return true;
    }
    else for ( XtringList::iterator it = mFilters.begin(); it!=mFilters.end(); ++ it ) {
            if ( *it == filter || *it == "(1 AND(" + filter + "))" ) {
                mFilters.erase ( it );
                return true;
            }
        }
    return false;
}

void dbRecord::addStructuralFilter(const Xtring& filter)
{
    if( !isStructuralFilter( filter ) )
        addFilter( "(1 AND(" + filter + "))" );
    else
        addFilter( filter );
}

bool dbRecord::isStructuralFilter(const Xtring& filter)
{
    return filter.startsWith( "(1 AND(" );
}

Xtring dbRecord::getJoinedFrom ( bool inner ) const
{
    Xtring ret = getTableName();
    Xtring join = inner ? " INNER JOIN " : " LEFT JOIN ";
    for( dbRecordRelationDict::const_iterator relit = mRecordRelations.begin();
            relit != mRecordRelations.end(); ++relit ) {
        dbRecordRelation *recrel = relit->second;
        ret += join + recrel->getRightTable()
               + " ON " + getTableName() + + "." + recrel->getLeftField()
               + "=" + recrel->getRightTable() + "." + recrel->getRightField();
    }
    return ret;
}

dbRecordID dbRecord::existsAnother ( const Xtring &condition ) const
{
    _GONG_DEBUG_ASSERT ( pConn );
    dbRecordID ret = 0;
    Xtring cond = condition;
    if ( cond.isEmpty() ) {
        for( uint nf = 0; nf < pTableDef->getFieldCount(); nf ++ ) {
            if( pTableDef->getFieldDefinition(nf)->isCode() )
                cond = pTableDef->getFieldDefinition(nf)->getName() +
                       "=" + pConn->toSQL( getValue(nf) );
        }
    }
    if ( cond.isEmpty() ) {
        _GONG_DEBUG_WARNING( "Table " + pTableDef->getName() + " has no code field" );
        return ret;
    }
    Xtring select = "SELECT " + getTableDefinition()->getFldIDName()
                    + " FROM " + getTableName()	+ getFilter( "WHERE", cond );
    ret = pConn->selectInt ( select );
    if ( !isNew() )
    {
        if ( ret == getRecordID() )
            return 0;
    }
    return ret;
}

int dbRecord::selectNextInt(const Xtring &fldname,
                            const Xtring &conds, bool usegaps ) const
{
    Xtring newconds = getFilter( "", conds);
    return pConn->selectNextInt( getTableName(), fldname, newconds, usegaps );
}


/**
 * @brief Seeks for a code and/or description with optional conditions
 *
 * @param fldcod The code field of the table
 * @param code the code value
 * @param flddesc The description field of the table
 * 		Can be the full qualified field of a related table: GROUP.NAME
 * @param desc the description value
 * @param cond any extra condition for the table
 * @param flags \sa dbRecord::SeekCodeFlags
 * @param matchingcond set with the first condition that returned rows
 * @return dbRecordID
 **/
dbRecordID dbRecord::seekCode( int &nvalues, const Xtring &fldcod, const Variant &code,
                               const Xtring &flddesc, const Variant &desc,
                               const Xtring &cond, SeekCodeFlags flags, Xtring &matchingcond )
{
    Xtring swheres[6];
    int nwheres=0, icond;
    dbRecordID recid=0;
    Xtring addcond = getFilter( "", cond );

    Xtring fullfldcod = getTableName() + "." + fldcod;
    Xtring binaryfullfldcod = "BINARY " + fullfldcod;
    Xtring fullflddesc;
    bool usejoinedtables = false;
    if( flddesc.find( ".") != Xtring::npos ) {
        usejoinedtables = true;
        fullflddesc = flddesc;
    } else {
        fullflddesc = getTableName() + "." + flddesc;
    }
    if ( !fldcod.isEmpty() && !flddesc.isEmpty() )
    {
        if ( !code.isEmpty() && !desc.isEmpty() )
        {
            swheres[nwheres++] = binaryfullfldcod + "=" + getConnection()->toSQL ( code )
                                 + " AND " + fullflddesc + "=" + getConnection()->toSQL ( desc );
            swheres[nwheres++] = binaryfullfldcod + "=" + getConnection()->toSQL ( code )
                                 + " AND " + getConnection()->toSQLLike ( fullflddesc, desc.toString() );
            swheres[nwheres++] = getConnection()->toSQLStartLike ( fullfldcod, code.toString() )
                                 + " AND " + fullflddesc + "=" + getConnection()->toSQL ( desc );
            swheres[nwheres++] = getConnection()->toSQLStartLike ( fullfldcod, code.toString() )
                                 + " AND " + getConnection()->toSQLLike ( fullflddesc, desc.toString() );
            swheres[nwheres++] = getConnection()->toSQLLike ( fullfldcod, code.toString() )
                                 + " AND " + getConnection()->toSQLLike ( fullflddesc, desc.toString() );
        }
        else if ( !code.isEmpty() )
        {
            swheres[nwheres++] = binaryfullfldcod + "=" + getConnection()->toSQL ( code );
            if( flags & SeekCodeInDesc ) {
                swheres[nwheres++] = getConnection()->toSQLLike ( getTableName() + "." + fldcod, code.toString() )
                                     + " OR " + getConnection()->toSQLLike ( fullflddesc, code.toString() );
            } else {
                swheres[nwheres++] = getConnection()->toSQLStartLike ( fullfldcod, code.toString() );
                swheres[nwheres++] = getConnection()->toSQLLike ( fullfldcod, code.toString() );
                swheres[nwheres++] = fullflddesc + "=" + getConnection()->toSQL ( code.toString() );
                swheres[nwheres++] = getConnection()->toSQLLike ( fullflddesc, code.toString() );
            }
        }
        else
        {
            swheres[nwheres++] = fullflddesc + "=" + getConnection()->toSQL ( desc );
            swheres[nwheres++] = getConnection()->toSQLLike ( fullflddesc, desc.toString() );
            swheres[nwheres++] = binaryfullfldcod + "=" + getConnection()->toSQL ( desc );
            swheres[nwheres++] = getConnection()->toSQLStartLike ( fullfldcod, desc.toString() );
            swheres[nwheres++] = getConnection()->toSQLLike ( fullfldcod, desc.toString() );
        }
    }
    else if ( !flddesc.isEmpty() )
    {
        swheres[nwheres++] = fullflddesc + "=" + getConnection()->toSQL ( desc );
        swheres[nwheres++] = getConnection()->toSQLLike ( fullflddesc, desc.toString() );
    }
    else
    {
        swheres[nwheres++] = binaryfullfldcod + "=" + getConnection()->toSQL ( code );
        swheres[nwheres++] = getConnection()->toSQLStartLike ( fullfldcod, code.toString() );
        swheres[nwheres++] = getConnection()->toSQLLike ( fullfldcod, code.toString() );
    }
    if( flags & SeekSecondaryCodes ) {
		Xtring searchstr = code.isEmpty() ? desc.toString() : code.toString();
		if( !searchstr.isEmpty() ) {
			Xtring secondary_cond;
			for( dbFieldDefinitionDict::const_iterator fldit = getTableDefinition()->getFieldDefinitions().begin();
				fldit != getTableDefinition()->getFieldDefinitions().end(); ++fldit ) {
				if( fldit->second->isSecondaryCode() ) {
					if( !secondary_cond.isEmpty() )
						secondary_cond += "AND";
					secondary_cond += "(" + getConnection()->toSQLLike(fldit->second->getName(), searchstr) + ")";
				}
			}
			if( !secondary_cond.isEmpty() ) {
				swheres[nwheres++] = secondary_cond;
			}
		}
	}
    nvalues = icond = 0;
    do
    {
        // The fields 'fldcod' & 'flddesc' are necessary for the choose form
        Variant vrecid = 0;
        Xtring select = "SELECT " + getTableName() + ".ID," + getTableName() + "." + fldcod;
        if( !fullflddesc.isEmpty() )
            select.append("," + fullflddesc);
        // If cond is not empty, chances are that this condition referres other tables
        select.append(" FROM " + ( (!cond.isEmpty() || usejoinedtables)
                                   ? getJoinedFrom( false /*left*/ ) : getTableName()));
        select.append(" WHERE ");
        if ( !addcond.isEmpty() )
            select.append( addcond + "AND" );
        select.append( "(" + swheres[icond] + ")" );
        try
        {
            Variant dummycode(code), dummydesc(desc);
            nvalues = getConnection()->selectValues ( select, &vrecid, &dummycode, &dummydesc );
            if ( nvalues == 1 ) {
                recid = vrecid.toInt();
                break;
            } else if( nvalues > 1 )
                break;
        }
        catch ( dbError &e )
        {
            nvalues = 0;
        }
        icond++;
    } while ( icond<nwheres && nvalues == 0 );
    if ( icond < nwheres ) {
        matchingcond = addcond;
        if( !matchingcond.isEmpty() )
            matchingcond += "AND ";
        matchingcond += "(" + swheres[icond] + ")";
    }
    if( recid != 0 ) {
        read( recid );
    }
    return recid;
}

}; // namespace gong
