/*<<<<<MODULE_INFO*/
// COPYLEFT dbFieldDefinition que representa una cuenta
// MEMBER isValid
// TYPE dbFieldDefinition contab::Cuenta
/*>>>>>MODULE_INFO*/

#include <dbappdbapplication.h>
#include "contabmodule.h"
/*<<<<<DBFIELD_CUENTA_INCLUDES*/
#include "contabfldcuenta.h"
/*>>>>>DBFIELD_CUENTA_INCLUDES*/

namespace gong {
namespace contab {

/*<<<<<DBFIELD_CUENTA_ISVALID*/
bool FldCuenta::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const
{
/*>>>>>DBFIELD_CUENTA_ISVALID*/
    Cuenta c(value->toString(), ModuleInstance->getDigitosTrabajo());
    if( c.isEmpty() && canBeNull() )
        return true;
    Xtring errormessage;
    bool valid = c.isValid(errormessage);
    if( !valid )
        if( integres )
            integres->addError(errormessage, this->getName() );
    return valid;
}

} // namespace contab
} // namespace gong
