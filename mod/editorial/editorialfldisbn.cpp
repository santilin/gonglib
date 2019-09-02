/*<<<<<MODULE_INFO*/
// COPYLEFT dbFieldDefinition que representa un ISBN
// MEMBER isValid
// TYPE dbFieldDefinition editorial::Isbn NoConstr
/*>>>>>MODULE_INFO*/

/*<<<<<DBFIELD_ISBN_INCLUDES*/
#include "editorialfldisbn.h"
/*>>>>>DBFIELD_ISBN_INCLUDES*/

/*
 Base de datos de ISBN de España:
 http://www.mcu.es/webISBN/tituloSimpleFilter.do?cache=init&prev_layout=busquedaisbn&layout=busquedaisbn&language=es
*/

namespace gong {
namespace editorial {

/*<<<<<DBFIELD_ISBN_ISVALID*/
bool FldIsbn::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const
{
/*>>>>>DBFIELD_ISBN_ISVALID*/
	return true;
}


}
}
