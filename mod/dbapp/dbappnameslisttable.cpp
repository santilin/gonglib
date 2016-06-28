

#include "dbappnameslisttable.h"
#include "dbappdbapplication.h"
#include <boost/type_traits/detail/is_function_ptr_helper.hpp>

namespace gong {

NamesListTable::InfoDict NamesListTable::mNamesListTables;

NamesListTable::NamesListTable( dbDefinition &db, const Xtring &name )
    : dbMasterTable( db, name )
{
    addFieldRecordID();
    addFieldIntCode( "CODIGO" )->setUnique( true );
    addFieldDesc( "NOMBRE" )->setUnique( true );
    addFieldInt( "TIPO" )->setSqlWidth(4)->setDefaultValue("0");
    addBehavior( DBAPP->getRecordTimestampBehavior() );
}

/**
 * @brief Sintax of the config string:
 *     Only values:    "Yes|No|Cancel" => { 1, "Yes", 1},{2, "No", 2}, {3, "Cancel", 3}
 *     Values and captions:  "1=Yes|3=No|6=Cancel" => { 1, "Yes", 1},{3, "No", 3}, {6, "Cancel", 6}
 *     Values and captions:  "1=Yes,1|2=No,1|3=Cancel,4" => { 1, "Yes", 1},{3, "No", 1}, {3, "Cancel", 4}
 *
 * @param conn ...
 * @return void
 **/
void NamesListTable::fillInfoList(dbConnection* conn)
{
    for( NamesListTable::InfoDict::const_iterator itt = mNamesListTables.begin();
            itt != mNamesListTables.end(); ++ itt ) {
        const Xtring &tablename = (*itt).first;
        Xtring values = DBAPP->getMachineSetting( "DBDEF.TABLE." + tablename + ".VALUES" ).toString();
        if( values.isEmpty() )
            _GONG_DEBUG_WARNING( "DBDEF.TABLE." + tablename + ".VALUES not found" );
        NamesListTable::Info *info = (*itt).second;
        // TODO: This connection should be the one from the module that defines the nameslisttable
        Xtring sql = "SELECT CODIGO, NOMBRE FROM " + conn->nameToSQL( tablename );
        dbResultSet *rs = conn->select( sql );
        while( rs->next() ) {
            info->values.push_back( rs->toInt((uint)0) );
            info->captions.push_back( rs->toString(1) );
        }
        if( info->values.size() == 0 ) {
            // Fill in from read only configuration
            // see void dbFieldListOfValues<int>::setValuesFromString(const Xtring &values)
            XtringList valuesandcaptions;
            Xtring values = DBAPP->getMachineSetting( "DBDEF.TABLE." + tablename + ".VALUES" ).toString();
            values.tokenize( valuesandcaptions, "|" );
            uint nitem = 1;
            dbRecord *r = DBAPP->createRecord( tablename );
            for( XtringList::const_iterator itv = valuesandcaptions.begin();
                    itv != valuesandcaptions.end();
                    ++itv, ++nitem ) {
                Xtring value, caption;
                (*itv).splitIn2( value, caption, "=" );
                if( caption.isEmpty() ) { // Empty caption, so caption is also the value
                    info->values.push_back( nitem );
                    info->captions.push_back( value );
                    info->types.push_back( nitem );
                    r->setValue( "CODIGO", nitem );
                    r->setValue( "NOMBRE", value );
                    r->setValue( "TIPO", nitem );
                } else {
                    Xtring scaption, stype;
                    caption.splitIn2( scaption, stype, "," );
                    if( stype.isEmpty() ) {
                        info->types.push_back( value.toInt() );
                        r->setValue( "TIPO", value.toInt() );
                    } else {
                        info->types.push_back( stype.toInt() );
                        r->setValue( "TIPO", stype.toInt() );
                    }
                    info->values.push_back( value.toInt() );
                    info->captions.push_back( caption );
                    r->setValue( "CODIGO", value.toInt() );
                    r->setValue( "NOMBRE", caption );
                }
                r->save(false, false );
                r->setNew( true );
            }
            delete r;
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
FldNamesListTable::FldNamesListTable(const Xtring &tablename, const Xtring& fldname,
                                     dbFieldDefinition::Flags flags, const Xtring& defaultvalue)
    : dbFieldListOfValues<int>( false, tablename, fldname,
                                SQLINTEGER, 5, 0, flags, defaultvalue ),
    mNamesListTableName( fldname )
{
    init();
}

FldNamesListTable::FldNamesListTable(const Variant &knameslisttable,
                                     const Xtring &tablename, const Xtring& fldname,
                                     dbFieldDefinition::Flags flags, const Xtring& defaultvalue)
    : dbFieldListOfValues<int>( false, tablename, fldname,
                                SQLINTEGER, 5, 0, flags, defaultvalue )
{
    mNamesListTableName = knameslisttable.toString();
    if( mNamesListTableName.isEmpty() )
        mNamesListTableName = fldname;
    init();
}

void FldNamesListTable::init()
{
    // info must be a pointer because it can not be created on the stack, as its fields will disappear off
    NamesListTable::InfoDict::const_iterator it = NamesListTable::getNamesListTables().find( mNamesListTableName );
    if(  it == NamesListTable::getNamesListTables().end() ) {
        NamesListTable::Info *info = new NamesListTable::Info();
        NamesListTable::getNamesListTables().insert( mNamesListTableName, info );
    }
    it = NamesListTable::getNamesListTables().find( mNamesListTableName );
    pListOfCaptions = const_cast<XtringList *>(&(*it).second->captions);
    pListOfValues = const_cast<IntList *>(&(*it).second->values);
}

void FldNamesListTable::fill( dbConnection &conn )
{
    pListOfCaptions->clear();
    pListOfValues->clear();
    Xtring sql = "SELECT CODIGO, NOMBRE FROM " + conn.nameToSQL( getName() );
    dbResultSet *rs = conn.select( sql );
    while( rs->next() ) {
        pListOfCaptions->push_back( rs->toString(1) );
        pListOfValues->push_back( rs->toInt((uint)0) );
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
        if( value->type() == Variant::tString ) {
            value->setValue( findCode( value->toString() ) );
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
    if( dbRecord::save(false, saverelated) ) {
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
    for( dbTableDefinitionDict::const_iterator tblit = DBAPP->getDatabase()->getTables().begin();
            tblit != DBAPP->getDatabase()->getTables().end(); ++tblit ) {
        for( dbFieldDefinitionDict::const_iterator fldit = tblit->second->getFieldDefinitions().begin();
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
    for( dbTableDefinitionDict::const_iterator tblit = DBAPP->getDatabase()->getTables().begin();
            tblit != DBAPP->getDatabase()->getTables().end(); ++tblit ) {
        for( dbFieldDefinitionDict::const_iterator fldit = tblit->second->getFieldDefinitions().begin();
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
