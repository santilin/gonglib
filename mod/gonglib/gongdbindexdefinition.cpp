/*<<<<<COPYLEFT*/
/** @file gongdbindexdefinition.cpp Index definitions for the data dictionary
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/

/*<<<<<MODULE_INFO*/
// COPYLEFT Index definitions for the data dictionary
// MEMBER Name Xtring rc
// MEMBER Unique bool rwc
// MEMBER Flags dbFieldDefinition::Flags rwc dbFieldDefinition::NONE
// MEMBER Options Xtring rwc 0
// MEMBER FieldDefinitions dbFieldDefinitionsList r
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
 * @param conn
 * @param tbldef
 * @param ignoreerrors
 * @return
 */
bool dbIndexDefinition::create( dbConnection *conn, const dbTableDefinition *tbldef,
                                bool ignoreerrors )
{
    Xtring select = "ALTER TABLE " + tbldef->getName() + " ADD";
    if( isUnique() )
        select += " UNIQUE";
    select += " INDEX " _GONG_INDEX_PREFIX + getName() + " (";
    for( unsigned int i = 0; i < getFieldCount(); i++ ) {
        if( i > 0 )
            select += ",";
        dbFieldDefinition *flddef = getFieldDefinition(i);
        select += flddef->getName();
        if( flddef->getSqlWidth() )
            select += "(" + conn->toSQL( flddef->getSqlWidth() ) + ")";
        if( flddef->isAscendent() )
            select += " ASC";
        else if( flddef->isAscendent() )
            select += " DESC";
    }
    select += ")";
    return conn->exec( select , ignoreerrors );
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
    Xtring select = Xtring("ALTER TABLE ") + tbldef->getName() + " DROP INDEX " _GONG_INDEX_PREFIX + getName();
    return conn->exec( select, ignoreerrors);
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
        out << *(const_cast<dbIndexDefinition &>(idxdef).getFieldDefinitions()[i]);
    return out;
}
#endif


}; // namespace




