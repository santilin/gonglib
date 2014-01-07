/*<<<<<MODULE_INFO*/
// COPYLEFT dbFieldDefinition que representa el id de la empresa en curso
// MEMBER isValid
// MEMBER customDefaultValue
// TYPE dbFieldDefinition empresa::EmpresaID NoConstr
/*>>>>>MODULE_INFO*/

#include <empresamodule.h>
/*<<<<<DBFIELD_EMPRESAID_INCLUDES*/
#include "empresafldempresaid.h"
/*>>>>>DBFIELD_EMPRESAID_INCLUDES*/

namespace gong
{
namespace empresa {


FldEmpresaID::FldEmpresaID(const Xtring& tablename, const Xtring& name,
                           dbFieldDefinition::Flags flags, const Xtring& reference)
    : dbFieldDefinition( tablename, name, SQLINTEGER, 11, 0,
                         flags | dbFieldDefinition::NOTNULL)
{
    setReference( reference );
}


/*<<<<<DBFIELD_EMPRESAID_ISVALID*/
bool FldEmpresaID::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const
{
/*>>>>>DBFIELD_EMPRESAID_ISVALID*/
    return true;
}

/*<<<<<DBFIELD_EMPRESAID_CUSTOMDEFAULTVALUE*/
Variant FldEmpresaID::customDefaultValue() const
{
/*>>>>>DBFIELD_EMPRESAID_CUSTOMDEFAULTVALUE*/
    return ModuleInstance->getEmpresaID();
}

} // Namespace empresa
} // Namespace gong


