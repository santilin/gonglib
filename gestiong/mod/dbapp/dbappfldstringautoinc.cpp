/*<<<<<MODULE_INFO*/
// COPYLEFT dbFieldDefinition para strings con autoincremento
// TYPE dbFieldDefinition StringAutoInc NoConstr
/*>>>>>MODULE_INFO*/
/*<<<<<DBFIELD_STRINGAUTOINC_INCLUDES*/
#include "dbappfldstringautoinc.h"
/*>>>>>DBFIELD_STRINGAUTOINC_INCLUDES*/

namespace gong {

Variant FldStringAutoInc::customDefaultValue() const
{
    Xtring res;
    if( !getDefaultValue().isEmpty() ) {
        Xtring sql = "SELECT MAX(LEFT(" + getFullName() + ", " + Xtring::number(getDefaultValue().length())
                     + ")) FROM " + pConn->nameToSQL( getTableName() )
                     + " WHERE " + pConn->toSQLLikeLiteral( getFullName(), getDefaultValue() + "%" )
                     + " ORDER BY 1";
        res = pConn->selectString( sql );
    }
    return Variant( Xtring::stringInc( res ) );
}

} // Namespace gong
