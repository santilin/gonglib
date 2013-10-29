/*<<<<<MODULE_INFO*/
// COPYLEFT dbFieldDefinition que representa un nombre pripio, de persona o asociación
// MEMBER isValid
// TYPE dbFieldDefinition contactos::NombrePropio
/*>>>>>MODULE_INFO*/
/*<<<<<DBFIELD_NOMBREPROPIO_INCLUDES*/
#include "contactosfldnombrepropio.h"
/*>>>>>DBFIELD_NOMBREPROPIO_INCLUDES*/

namespace gong {
namespace contactos {

/*<<<<<DBFIELD_NOMBREPROPIO_ISVALID*/
bool FldNombrePropio::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const
{
    /*>>>>>DBFIELD_NOMBREPROPIO_ISVALID*/
    return true;
}

} // namespace contactos
} // namespace gong
