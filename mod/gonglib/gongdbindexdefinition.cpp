/*<<<<<MODULE_INFO*/
// COPYLEFT Index definitions for the data dictionary
// MEMBER Name Xtring rc
// MEMBER Unique bool rwc
// MEMBER Flags dbFieldDefinition::Flags rwc dbFieldDefinition::NONE
// MEMBER Options Xtring rwc 0
// MEMBER FieldDefinitions dbFieldDefinitionDict r
// TYPE Class dbIndexDefinition fullconstr set_returns_reference
/*>>>>>MODULE_INFO*/

/**
	@class gong::dbIndexDefinition
	@brief Index definitions for the data dictionary

	Holds the definition of an index for a table.
	It can contain multiple field definitions, added with \a addField

	Once created the definition, you can \a create or \a drop it from the database SQL server.

	@see dbDefinition, dbFieldDefinition
*/


#include "gongdebug.h"
#include "gongdbconnection.h"
#include "gongdbtabledefinition.h"
#include "gongdbindexdefinition.h"

namespace gong
{

/**
 * Creates an index. Prepends its name with _gong_ so that the reindex function can drop
 * all these indexes while preserving user created ones
 * SqlLite3: The index name must include the tablename, as the DROP INDEX cant delete from a given table
 * @param conn
 * @param tbldef
 * @param ignoreerrors
 * @return
 */
bool dbIndexDefinition::create( dbConnection *conn, const dbTableDefinition *tbldef,
                                bool ignoreerrors )
{
    Xtring ddl;
    if( conn->isMySQL() )
        ddl = "ALTER TABLE " + tbldef->getName() + " ADD";
    else if( conn->isSQLite() )
        ddl = "CREATE";
    if( isUnique() )
        ddl += " UNIQUE";
    ddl += " INDEX " _GONG_INDEX_PREFIX;
    if( conn->isSQLite() )
        ddl += tbldef->getName() + "_" + getName() + " ON " + tbldef->getName();
    else
        ddl += getName();
    ddl += " (";
    for( unsigned int i = 0; i < getFieldCount(); i++ ) {
        if( i > 0 )
            ddl += ",";
        dbFieldDefinition *flddef = getFieldDefinition(i);
        ddl += flddef->getName();
        if( flddef->getSqlWidth() )
            ddl += "(" + conn->toSQL( flddef->getSqlWidth() ) + ")";
        if( flddef->isAscendent() )
            ddl += " ASC";
        else if( flddef->isAscendent() )
            ddl += " DESC";
    }
    ddl += ")";
    return conn->exec( ddl, ignoreerrors );
}


/**
 * Deletes an index, taking into account the _gong_ prefix added by create
 * @param conn
 * @param tbldef
 * @param ignoreerrors
 * @return
 */
bool dbIndexDefinition::drop( dbConnection *conn, const dbTableDefinition *tbldef,
                              bool ignoreerrors )
{
    Xtring ddl;
    if( conn->isSQLite() )
        ddl = Xtring("DROP INDEX IF EXISTS ") + _GONG_INDEX_PREFIX + tbldef->getName() + "_" + getName();
    else
        ddl = Xtring("ALTER TABLE ") + tbldef->getName() + " DROP INDEX " _GONG_INDEX_PREFIX + getName();
    return conn->exec( ddl, ignoreerrors);
}

dbFieldDefinition *dbIndexDefinition::addField(const dbFieldDefinition *fielddef)
{
    mFieldDefinitions.insert(fielddef->getName(), const_cast<dbFieldDefinition *>(fielddef) );
    return const_cast<dbFieldDefinition *>(fielddef);
}

dbFieldDefinition *dbIndexDefinition::addField(const Xtring &name,
        unsigned short int width, bool asc)
{
    dbFieldDefinition *fielddef = new dbFieldDefinition(Xtring::null, name, SQLSTRING, width, 0,
            (asc? dbFieldDefinition::ASCENDENT : dbFieldDefinition::DESCENDENT ) );
    mFieldDefinitions.insert(fielddef->getName(), const_cast<dbFieldDefinition *>(fielddef) );
    return const_cast<dbFieldDefinition *>(fielddef);
}


#ifdef _GONG_DEBUG
std::ostream &operator<<(std::ostream &out, const dbIndexDefinition &idxdef)
{
    out << '\t' << idxdef.getName() << std::endl;
    for( unsigned int i = 0; i < idxdef.getFieldDefinitions().size(); i++ )
        out << *(const_cast<dbIndexDefinition &>(idxdef).getFieldDefinitions().seq_at(i));
    return out;
}
#endif


}; // namespace




