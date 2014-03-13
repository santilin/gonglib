#include "config.h"
#include "gonggettext.h"
#include "gongregexp.h"
#include "gongdbdefinition.h"
#include "gongcsvutils.h"
#include "gongdbrecord.h"
#include "gongdbrecordlist.h"
#include "gonglibrary.h"
#include "gongdbfieldlistofvalues.h"
#include "gongxmlparser.h"
#include <boost/concept_check.hpp>

namespace gong {

class FugitRecordImporter: public XmlParser
{
public:
    FugitRecordImporter(dbRecord *record, dbDefinition *dbdef)
        : XmlParser(), mLevel(0), pRecord(0), pdbDefinition( dbdef )
    {
        pRecords[0] = record;
    }

private:
#ifdef HAVE_POCOLIB
	virtual void startElement( const Xtring &name, const Attributes &attrList );
	virtual void endElement( const Xtring &name );
	virtual void characters( const Xtring &characters );
#else
    virtual void startElement( const xmlChar *name, const xmlChar **attributes ); // From XmlParser
    virtual void endElement( const xmlChar *name ); // From XmlParser
    virtual void characters( const xmlChar *text, unsigned int len ); // From XmlParser
#endif

    int mLevel;
    Xtring mFldData;
    Xtring mFldName;
    Xtring mCurTable;
    dbRecord *pRecord, *pRecords[10];
    dbDefinition *pdbDefinition;
};

#ifdef HAVE_POCOLIB
void FugitRecordImporter::characters( const Xtring &data )
#else
void FugitRecordImporter::characters(const xmlChar *text, unsigned int len)
#endif
{
    if( mCurTable.isEmpty() || mFldName.isEmpty() ) {
//		_GONG_DEBUG_PRINT(0, Xtring::printf("table=%s,fld=%s: Skipping", mCurTable.c_str(), mFldName.c_str()) );
        return;
    } else {
#ifndef HAVE_POCOLIB
        Xtring data( (const char *)text, len);
#endif
//		_GONG_DEBUG_PRINT(0, Xtring::printf("[[{%s}]],%d fld=%s", data.c_str(), len, mFldName.c_str()) );
        mFldData += data;
    }
}


#ifdef HAVE_POCOLIB
void FugitRecordImporter::startElement( const Xtring &name, const Attributes &attributes )
#else
void FugitRecordImporter::startElement( const xmlChar *name, const xmlChar **attributes )
#endif
{
// 	_GONG_DEBUG_PRINT(0, Xtring::printf("level=%d,%s,%s", mLevel, name, (attributes)?((const char *)*attributes):"(null attributes)"));
    const Xtring tagname = Xtring(name).upper();
    mFldData.clear();
    if( tagname.startsWith("REGISTRO-") ) {
        mCurTable = tagname.mid(9);
        if( mLevel == 0 )
            pRecord = pRecords[0];
        if( mLevel > 0 ) {
            pRecord = pRecords[ mLevel - 1 ];
            dbRecordRelation *rel = pRecord->findRelationByRelatedTable( mCurTable );
            if( rel ) {
                pRecords[mLevel] = pRecord = new dbRecord( pRecords[0]->getConnection(),
                        pdbDefinition->findTableDefinition(mCurTable) );
                rel->addRelatedRecord( pRecord );
            } else {
                _GONG_DEBUG_WARNING( "Related table " + mCurTable + " not found.");
                stopParser( false );
            }
        }
        mLevel++;
    }
    else if( mLevel == 0 ) {
        _GONG_DEBUG_WARNING( Xtring::printf(
                                 _("TAG '%s' debe comenzar con REGISTRO-"), tagname.c_str() ) );
        stopParser( false );
    } else {
        mFldName = tagname;
    }
}

#ifdef HAVE_POCOLIB
void FugitRecordImporter::endElement( const Xtring &name )
#else
void FugitRecordImporter::endElement( const xmlChar *name )
#endif
{
//  	_GONG_DEBUG_PRINT(0, Xtring::printf("level=%d,%s", mLevel, name));
    const Xtring tagname = Xtring(name).upper();
    if( tagname.startsWith("REGISTRO-") ) {
        mLevel--;
    } else if( tagname == mFldName ) {
        if( pRecord ) {
//			_GONG_DEBUG_PRINT(0, Xtring::printf("%s.%s = %s", mCurTable.c_str(), tagname.c_str(), mFldData.trim().c_str()) );
            char *lnum_save = strdup(setlocale( LC_NUMERIC, NULL ));
            setlocale( LC_NUMERIC, "C" );
            char *ltime_save = strdup(setlocale( LC_TIME, NULL ));
            setlocale( LC_TIME, "C" );
            if( !pRecord->setValue(tagname, mFldData.trim()) ) {
                _GONG_DEBUG_WARNING(Xtring::printf( _("Campo '%s' desconocido para el fichero '%s'"),
                                                    tagname.c_str(), pRecord->getTableName().c_str() ));
            }
            setlocale( LC_NUMERIC, lnum_save );
            setlocale( LC_TIME, ltime_save );
            free( ltime_save );
            free( lnum_save );
        }
    } else {
        _GONG_DEBUG_WARNING( Xtring::printf( _("Etiqueta inesperada '%s' a nivel %d"),
                                             tagname.c_str(), mLevel) );
    }
    return;
}

Xtring dbRecord::toString ( int format, const Xtring &includedFields ) const
{
    Xtring text;
    unsigned int i;
    if ( format == TOSTRING_DEBUG_COMPLETE )
    {
        if( isNew() )
            text += "(new)";
        for ( i=0; i<getFieldCount(); i++ )
        {
            if( getFieldDefinition(i)->getSqlColumnType() == SQLBLOB )
                continue;
            if ( i!=0 )
                text += ",";
            text += getFieldDefinition ( i )->getName() + "=";
            if( isNullValue(i) )
                text += "NULL";
            else
                text += getValue ( i ).toString();
        }
    }
    else if ( format == TOSTRING_DEBUG_ONLY_MODIFIED )
    {
        if( isNew() )
            text += "(new)";
        for ( i=0; i<getFieldCount(); i++ )
        {
			const dbFieldDefinition *flddef = getFieldDefinition(i);
            if( flddef->getSqlColumnType() == SQLBLOB )
                continue;
            if( !mFieldValues.seq_at(i)->isModified() )
                continue;
            if ( i!=0 )
                text += ",";
            text += flddef->getName() + "=";
            if( isNullValue(i) )
                text += "NULL";
            else
                text += getValue(i).toString();
        }
    }
    else if ( format == TOSTRING_DEBUG_COMPLETE_WITH_RELATIONS )
    {
        text += getTableName();
        if( isNew() )
            text += "(new)";
        text += ": ";
        for ( i=0; i<getFieldCount(); i++ )
        {
			const dbFieldDefinition *flddef = getFieldDefinition(i);
            if( flddef->getSqlColumnType() == SQLBLOB )
                continue;
            if ( i!=0 )
                text += ",";
            text += flddef->getName() + "=" + getValue(i).toString();
        }
		for( dbRecordRelationDict::const_iterator relit = mRecordRelations.begin();
			relit != mRecordRelations.end(); ++relit ) {
			dbRecordRelation *recrel = relit->second;
            if ( recrel->isEnabled() )
            {
                if ( recrel->getType() == dbRelationDefinition::one2many )
                {
                    Variant leftvalue = getValue ( recrel->getLeftField() );
                    for ( dbRecordList::iterator detail = recrel->getRelatedRecordList()->begin();
                            detail != recrel->getRelatedRecordList()->end();
                            ++detail )
                    {
                        text += "\n\t(" + Xtring::number ( i+1 ) + "/" + Xtring::number(recrel->getRelatedRecordList()->size())
                                + "): " + ( *detail )->toString ( format, includedFields );
                    }
				} else if ( recrel->getType() == dbRelationDefinition::aggregate ) {
 					text += "\n\t" + recrel->getLeftField() + ": "
 					        + recrel->getRelatedRecord(-1)->toString ( format, includedFields );
				}
            }
        }
    }
    else if ( format == TOSTRING_FUGIT )
    {
        text = "<REGISTRO-" + getTableName().upper() + " id='" + Xtring::number ( getRecordID() ) + "'>\n";
        char *lnum_save = strdup(setlocale( LC_NUMERIC, NULL ));
        setlocale( LC_NUMERIC, "C" );
        char *ltime_save = strdup(setlocale( LC_TIME, NULL ));
        setlocale( LC_TIME, "C" );
        for ( i=1; i<getFieldCount(); i++ )
        {
#if OJO /// \todo {0.4}  We still don't have regular expressions
            if ( !includedFields.isEmpty() )
            {
                if ( !includedFields.exactMatch ( getFullFieldName ( i ) ) )
                    continue;
            }
#endif
            if ( getFieldDefinition ( i )->getName().left ( 4 ).lower() != "rec_" )
            {
                Variant value = getValue ( i );
                text += "\t<" + getFieldDefinition ( i )->getName().upper()
                        //  	      + "' type='" + value.typeToName(value.type())
                        + ">";
                Xtring svalue;
                switch( value.type() ) {
                case Variant::tMoney:
                    svalue = Xtring::number( value.toDouble() );
                    break;
                default:
                    svalue = value.toString();
                    break;
                }
                text += svalue.XMLProtect() + "</" + getFieldDefinition ( i )->getName().upper() + ">\n";
            }
        }
		for( dbRecordRelationDict::const_iterator relit = mRecordRelations.begin();
			relit != mRecordRelations.end(); ++relit ) {
			dbRecordRelation *recrel = relit->second;
            if ( recrel->isEnabled() )
            {
                if ( recrel->getType() == dbRelationDefinition::one2many )
                {
                    Variant leftvalue = getValue ( recrel->getLeftField() );
                    for ( dbRecordList::iterator detail = recrel->getRelatedRecordList()->begin();
                            detail != recrel->getRelatedRecordList()->end();
                            ++detail )
                    {
                        text += ( *detail )->toString ( format, includedFields );
                    }
                }
            }
        }
        setlocale( LC_TIME, ltime_save );
        setlocale( LC_NUMERIC, lnum_save );
        free( ltime_save );
        free( lnum_save );
        text += "</REGISTRO-" + getTableName().upper() + ">\n";
    }
    else if ( format == TOSTRING_CSV )
    {
        for ( i=0; i<getFieldCount(); i++ ) {
            if ( i!=0 )
                text += ",";
            Variant value = getValue ( i );
            switch ( value.type() )
            {
            case Variant::tDate:
            case Variant::tDateTime:
            case Variant::tTime:
            case Variant::tString:
                text += "\"" + CsvUtils::dupQuotes(value.toString(), '"') + "\"";
                break;
            default:
                text += value.toString();
            }
        }
        text += "\n";
    }
    else if ( format == TOSTRING_DEBUG )
        text = "ID=" + Xtring::number ( getRecordID() );
    else if ( format == TOSTRING_CODE_AND_DESC )
    {
        for ( i=0; i<getFieldCount(); i++ )
        {
            const dbFieldDefinition *flddef = getFieldDefinition ( i );
            if ( ( flddef->isCode() || flddef->isDescription() )
                    && ( !flddef->isPrimaryKey() ) )
            {
                if ( !text.isEmpty() )
                    text += ", ";
                text += getValue ( i ).toString();
            }
        }
    }
    else if ( format == TOSTRING_CODE_AND_DESC_WITH_TABLENAME )
    {
        // code and description
        for ( i=0; i<getFieldCount(); i++ )
        {
            const dbFieldDefinition *flddef = getFieldDefinition ( i );
            if ( ( flddef->isCode() || flddef->isDescription() )
                    && ( !flddef->isPrimaryKey() ) )
            {
                if ( !text.isEmpty() )
                    text += ", ";
                text += getValue ( i ).toString();
            }
        }
        text = getTableDefinition()->getDescSingular() + ": " + text;
    }
    return text;
}

bool dbRecord::fromString ( const Xtring &source, int format, const Xtring &includedFields )
{
    // Si un valor está en blanco, no se importa, se conserva el valor que tenía.
    // Para forzar un valor a estar vacío, debe tener el valor ~
    if ( format == TOSTRING_CSV ) 
    {
        _GONG_DEBUG_PRINT ( 4, "Import to " + getTableName() + " from " + source );
        XtringList names, values, related_tables;
        CsvUtils::tokenize( values, source, '\"', ',' );
        CsvUtils::tokenize( names, includedFields, '\"', ',' );
		uint i = 0;
        for ( XtringList::const_iterator namesit = names.begin();
                namesit != names.end(); ++namesit) {
            Xtring fldname = *namesit;
			if( i >= values.size() ) {
                _GONG_DEBUG_WARNING( "Importing CSV for table " + getTableName() + ": there is no value for field " + fldname );
				continue;
			}
            Xtring fldvalue = values[i++];
            if( fldvalue == "~" ) {
                setValue( fldname, Variant() );
            } else if( !fldvalue.isEmpty() ) {
                dbFieldDefinition *flddef = getTableDefinition()->findFieldDefinition(fldname, false);
                if( !flddef ) {
					Xtring reftable, reffield;
					fldname.splitIn2(reftable, reffield, ".");
					if ( !reftable.isEmpty() && reftable != getTableName() ) {
						dbTableDefinition *tbldef = getTableDefinition()->getdbDefinition().findTableDefinition( reftable );
						if( tbldef ) {
							flddef = tbldef->findFieldDefinition( reffield );
							related_tables << reftable;
						} else {
							_GONG_DEBUG_WARNING("Related field " + fldname + " not found in table " + getTableName() );
						}
					}
				}
				if( flddef ) {
                    if( dbFieldListOfValues<int> *fldlov = dynamic_cast< dbFieldListOfValues<int> *>(flddef) ) {
                        int value = fldlov->findValue( fldvalue );
                        if( value == 0 )
                            value = fldvalue.toInt();
                        setValue( fldname, fldvalue );
                    } else {
						setValue( fldname, fldvalue );
					}
                } else {
					setValue( fldname, fldvalue );
				}
            }
        }
        // Find or create related records
        for( XtringList::const_iterator relit = related_tables.begin();
			relit != related_tables.end(); ++relit ) {
			dbRecordRelation *relation = findRelationByRelatedTable( *relit );
			if( relation ) {
				dbRecord *relrecord = relation->getRelatedRecord();
				if( relrecord ) {
					/// TODO: Refactor, make this a function as it it used in existAnother
					Xtring codecond;
					for ( unsigned int nf = 0; nf < mFieldValues.size(); nf ++ )
					{
						const dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( nf );
						if( !flddef->isPrimaryKey() && flddef->isCode() )
						{
							Variant fldvalue = getValue( *relit + "." + flddef->getName() );
							if( !( flddef->canBeNull() && fldvalue.isEmpty() ) )
							{
								if ( !codecond.isEmpty() )
									codecond += "OR";
								codecond += "(" + getConnection()->nameToSQL ( flddef->getName() )
											+ "=" + getConnection()->toSQL ( fldvalue ) + ")";
							}
						}
					}
					dbRecordID recid = 0;
					if( !codecond.isEmpty() ) {
						recid = relrecord->getConnection()->selectInt( "SELECT ID FROM " + relrecord->getTableName()
								+ relrecord->getFilter("WHERE", codecond ) );
					}
					if( recid == 0 && relation->getType() == dbRelationDefinition::aggregate ) {
						if( relrecord->save(false) )
							recid = relrecord->getRecordID();
					}
					setValue( *relit + "_ID", recid );
				} else {
					_GONG_DEBUG_WARNING("In table " + getTableName() + ", related record for table " + *relit + " not found");
				}
			} else {
				_GONG_DEBUG_WARNING("In table " + getTableName() + ", relation for table " + *relit + " not found" );
			}
		}					
    }
    else if ( format == TOSTRING_FUGIT )
    {
#if defined( HAVE_POCOLIB ) or defined( HAVE_LIBXML2 )
        FugitRecordImporter recimporter( this, &getTableDefinition()->getdbDefinition() );
        recimporter.parseString( source.c_str() );
#else
		throw std::runtime_error( "Xml not supported in dbRecord::toString()" );
#endif
    }
    return true;
}

bool dbRecord::isValid(ValidResult::Context context, ValidResult *result)
{
    _GONG_DEBUG_PRINT(10, toString ( TOSTRING_DEBUG_COMPLETE ) );
    if( context == ValidResult::saving ) {
        if ( getRecordID() == 0 && !mIsNew && !mIsDeleted ) {
            // If this happens, the record is seriously damaged
            // Moreover, it cant be read by other related records
            if ( result )
                result->addError( _( "Existing records can't have ID=0" ),
                                  getTableDefinition()->getFldIDName() );
            return false;
        }
        Variant creadate = getValue ( "REC_FECHA_CREA" );
        Variant moddate = getValue ( "REC_FECHA_MODIF" );
        // Even though all records should have the following fields: REC_FECHA_CREA,
        // REC_FECHA_MODIF y REC_USUAR_MODIF, some of them might don't have them
        if ( moddate.isValid() && creadate.isValid() ) {
            if ( moddate.toDateTime() < creadate.toDateTime() ) {
                // If this happens, there is an error in the record, but we cant consider it fatal.
                // It might be a program failure or an db admin update
                _GONG_DEBUG_WARNING ( getTableName()
                                      + ": Fecha de modificación anterior a la de creación : " + 	toString() );
                setValue ( "REC_FECHA_MODIF", moddate );
            }
        }
    }
    bool ret = true;
    for ( unsigned int nf = 1 /* Skip ID */; nf < mFieldValues.size(); nf ++ )
    {
        const dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( nf );
// 		_GONG_DEBUG_PRINT(0, Xtring::printf("Validating '%s', value='%s'",
// 					flddef->getFullName().c_str(), getValue( nf ).toString().c_str() ) );
        if ( !flddef->isValid ( this, mFieldValues.seq_at(nf), context, result ) ) {
#ifdef _GONG_DEBUG
            if( result && result->count() ) {
                _GONG_DEBUG_PRINT(3, Xtring::printf("Field '%s' is not valid: '%s'",
                                                    flddef->getFullName().c_str(),
                                                    result ? "" : result->getMessageInfo(0).message.c_str() ) );
            } else {
                _GONG_DEBUG_PRINT(3, Xtring::printf("Field '%s' is not valid",
                                                    flddef->getFullName().c_str() ) );
            }
#endif
            ret = false;
        }
    }
    return ret;
}


/**
 * Looks for another record equal or similar to this one.
 * This function is virtual.
 * If there is an unique field whose value match this record, then sets matchingid to that record and returns true
 * If not, looks for a similar one in the first unique field and returns false
 *
 * In MySQL, this condition:  WHERE FIELD LIKE '%' does not return rows where FIELD IS NULL
 * So, if the field can be null, we supress that condition
 *
 * @param matchingid
 * @return
 */
bool dbRecord::findMatchingRecord(dbRecordID *matchingid ) const
{
    *matchingid = 0;
    // Search based upon unique fields
    Xtring cond;
    for ( unsigned int nf = 0; nf < mFieldValues.size(); nf ++ )
    {
        const dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( nf );
        if ( !flddef->isPrimaryKey() && flddef->isCode() )
        {
            if ( ! ( flddef->canBeNull() && getValue ( flddef->getName() ).isEmpty() ) )
            {
                if ( !cond.isEmpty() )
                    cond += "OR";
                cond += "(" + getConnection()->nameToSQL ( flddef->getName() )
                        + "=" + getConnection()->toSQL ( getValue ( flddef->getName() ) ) + ")";
            }
        }
    }
    *matchingid = existsAnother ( cond );
    if ( *matchingid != 0 )
        return true;

    // Search based upon unique indexes
    cond.clear();
// 	for ( unsigned int ni = 0; ni < pTableDef->getIndexDefinitions().size(); ni++ )	{
// 		const dbIndexDefinition *idef = pTableDef->getIndexDefinition( ni );
// 		if( idef->isUnique() ) {
// 			for ( unsigned int nf = 0; nf < idef->getFieldDefinitions().size(); nf ++ ) {
// 				const dbFieldDefinition *flddef = idef->getFieldDefinition ( nf );
// 				if ( ! ( flddef->canBeNull() && getValue ( flddef->getName() ).isEmpty() ) ) {
// 					if ( !cond.isEmpty() )
// 						cond += "AND";
// 					cond += "(" + getConnection()->nameToSQL ( flddef->getName() )
// 							+ "=" + getConnection()->toSQL ( getValue ( flddef->getName() ) ) + ")";
// 				}
// 			}
// 			*matchingid = existsAnother ( cond );
// 			if ( *matchingid != 0 )
// 				return true;
// 		}
// 	}
    /*
    	// Look for starting like
    	cond.clear();
    	if ( *matchingid == 0 )
    	{
    		for ( unsigned int nf = 0; nf < mFieldValues.size(); nf ++ )
    		{
    			const dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( nf );
    			if ( !flddef->isPrimaryKey() && flddef->isUnique() )
    			{
    				if ( ! ( flddef->canBeNull() && getValue ( flddef->getName() ).isEmpty() ) )
    				{
    					if ( !cond.isEmpty() )
    						cond += "AND";
    					cond += "(" + pConn->toSQLStartLike ( flddef->getName(), getValue ( flddef->getName() ).toString() ) + ")";
    				}
    			}
    		}
    	}
    	*matchingid = existsAnother ( cond );
    	if ( *matchingid != 0 )
    		return false;
    	// Look for like
    	cond.clear();
    	if ( *matchingid == 0 )
    	{
    		for ( unsigned int nf = 0; nf < mFieldValues.size(); nf ++ )
    		{
    			const dbFieldDefinition *flddef = pTableDef->getFieldDefinition ( nf );
    			if ( !flddef->isPrimaryKey() && flddef->isUnique() )
    			{
    				if ( ! ( flddef->canBeNull() && getValue ( flddef->getName() ).isEmpty() ) )
    				{
    					if ( !cond.isEmpty() )
    						cond += "AND";
    					cond += "(" + pConn->toSQLLike ( flddef->getName(), getValue ( flddef->getName() ).toString() ) + ")";
    				}
    			}
    		}
    	}
    	*matchingid = existsAnother ( cond );*/
    return false;
}

/// \todo {0.4} class dbRecordFactory
dbRecord *GongLibrary::createRecord(const Xtring &tablename, dbRecordID recid, dbUser *user)
{
    _GONG_DEBUG_WARNING(__PRETTY_FUNCTION__);
    return 0;
}


}
