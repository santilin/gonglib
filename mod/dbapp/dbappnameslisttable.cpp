#include "dbappnameslisttable.h"
#include "dbappdbapplication.h"
#include <boost/type_traits/detail/is_function_ptr_helper.hpp>

namespace gong {

NamesListTable::InfoList NamesListTable::mNamesListTables;

NamesListTable::NamesListTable( dbDefinition &db, const Xtring &name )
    : dbMasterTable( db, name )
{
    addFieldRecordID();
    addFieldIntCode( "CODIGO" )->setUnique( true );
    addFieldDesc( "NOMBRE" )->setUnique( true );
    addBehavior( DBAPP->getRecordTimestampBehavior() );
}

void NamesListTable::fillInfoList(dbConnection* conn)
{
	for( NamesListTable::InfoList::const_iterator it = mNamesListTables.begin();
		it != mNamesListTables.end(); ++ it ) {
		_GONG_DEBUG_PRINT(0, (*it).first );
		NamesListTable::Info *info = (*it).second;
		// TODO: This connection should be the one from the module that defines the nameslisttable
		Xtring sql = "SELECT CODIGO, NOMBRE FROM " + conn->nameToSQL( (*it).first );
		dbResultSet *rs = conn->select( sql );
		while( rs->next() ) {
			info->values.push_back( rs->toInt(0) );
			info->captions.push_back( rs->toString(1) );
			_GONG_DEBUG_PRINT(0, info->captions.join(",") );
		}
	}
}


/**
 * @brief We can not pass the FldNamesListTable mCaptions and mValues to the constructor of
 *        dbFieldListOfValues<int> as the former ones are not created yet and the latter
 * 		  iterates on them to add the integer values
 *
 * @param tablename ...
 * @param fldname ...
 * @param flags ...
 * @param defaultvalue ...
 **/
FldNamesListTable::FldNamesListTable(const Xtring& tablename, const Xtring& fldname,
                                     dbFieldDefinition::Flags flags, const Xtring& defaultvalue)
    : dbFieldListOfValues<int>( false, tablename, fldname,
                                SQLINTEGER, 5, 0, flags, defaultvalue )
{
	// TODO destructor to delete these infos
	NamesListTable::Info *info = new NamesListTable::Info();
	info->captions << "Prueba";
	NamesListTable::InfoList::const_iterator it = NamesListTable::getNamesListTables().find( fldname );
	if(  it == NamesListTable::getNamesListTables().end() ) {
		NamesListTable::getNamesListTables().insert( fldname, info );
NamesListTable::InfoList::const_iterator it2 = NamesListTable::getNamesListTables().find( fldname );
		_GONG_DEBUG_PRINT(0, (*it2).second->captions.join(",") );

	} else {
		info = (*it).second;
	}
	pListOfCaptions = &info->captions;
	pListOfValues = &info->values;
}

void FldNamesListTable::fill( dbConnection &conn )
{
    pListOfCaptions->clear();
    pListOfValues->clear();
    Xtring sql = "SELECT CODIGO, NOMBRE FROM " + conn.nameToSQL( getName() );
    dbResultSet *rs = conn.select( sql );
    while( rs->next() ) {
        pListOfCaptions->push_back( rs->toString(1) );
        pListOfValues->push_back( rs->toInt(0) );
    }
}

int FldNamesListTable::findCode(const Xtring& name) const
{
    int nv = 0;
    for( XtringList::const_iterator capit = getListOfCaptions().begin();
            capit != getListOfCaptions().end(); ++capit ) {
        if( Xtring(*capit).upper() == name.upper() )
            return nv;
        ++nv;
    }
    return nv;
}


bool FldNamesListTable::isValid( dbRecord *r, dbFieldValue *value,
                                 ValidResult::Context context, ValidResult *integres) const
{
	/// TODO: check that the field values contains value
    bool ret = dbFieldDefinition::isValid(r, value, context, integres);
    if( context == ValidResult::fixing ) {
        if( value->toVariant().type() == Variant::tString ) {
            value->setValue( findCode( value->toVariant().toString() ) );
        }
    }
    return ret;
}

RecNamesListTable::RecNamesListTable(const Xtring& tablename, dbConnection* conn, dbRecordID recid, dbUser* user)
    : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition(tablename), recid, user)
{
}

bool RecNamesListTable::save(bool saverelated)
{
    if( dbRecord::save(saverelated) ) {
        updateDatabaseFields();
        return true;
    } else {
        return false;
    }
}

bool RecNamesListTable::remove()
{
    if( dbRecord::remove() ) {
        updateDatabaseFields();
        return true;
    } else {
        return false;
    }
}

void RecNamesListTable::updateDatabaseFields()
{
    for( dbTableDefinitionsList::const_iterator tblit = DBAPP->getDatabase()->getTables().begin();
            tblit != DBAPP->getDatabase()->getTables().end(); ++tblit ) {
        for( dbFieldDefinitionsList::const_iterator fldit = tblit->second->getFieldDefinitions().begin();
                fldit != tblit->second->getFieldDefinitions().end(); ++fldit ) {
            if( fldit->second->getName() == getTableName() ) {
                if( FldNamesListTable *fldlt = dynamic_cast<FldNamesListTable *>(fldit->second) ) {
                    fldlt->fill( *getConnection() );
                }
            }
        }
    }
}

/**
 * @brief finds the code for a given name in the first nameslisttable field of the database
 *
 * @param name ...
 * @return int
 **/
int RecNamesListTable::findCode(const Xtring& name) const
{
    int ret = 0;
    for( dbTableDefinitionsList::const_iterator tblit = DBAPP->getDatabase()->getTables().begin();
            tblit != DBAPP->getDatabase()->getTables().end(); ++tblit ) {
        for( dbFieldDefinitionsList::const_iterator fldit = tblit->second->getFieldDefinitions().begin();
                fldit != tblit->second->getFieldDefinitions().end(); ++fldit ) {
            if( fldit->second->getName() == getTableName() ) {
                if( FldNamesListTable *fldlt = dynamic_cast<FldNamesListTable *>(fldit->second) ) {
                    return fldlt->findCode( name );
                }
            }
        }
    }
    return ret;
}


} // namespace
