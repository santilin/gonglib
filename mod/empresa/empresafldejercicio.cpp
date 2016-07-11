/*<<<<<MODULE_INFO*/
// COPYLEFT dbFieldDefinition que representa el ejercicio en curso
// MEMBER isValid
// MEMBER customDefaultValue
// TYPE dbFieldDefinition empresa::Ejercicio NoConstr
/*>>>>>MODULE_INFO*/

#include "empresamodule.h"
/*<<<<<DBFIELD_EJERCICIO_INCLUDES*/
#include "empresafldejercicio.h"
/*>>>>>DBFIELD_EJERCICIO_INCLUDES*/

namespace gong {
namespace empresa {

/*<<<<<DBFIELD_EJERCICIO_ISVALID*/
bool FldEjercicio::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const
{
/*>>>>>DBFIELD_EJERCICIO_ISVALID*/
	if( context == ValidResult::saving && value->toInt() == 0 )
		return false;
    return true;
}

/*<<<<<DBFIELD_EJERCICIO_CUSTOMDEFAULTVALUE*/
Variant FldEjercicio::customDefaultValue() const
{
/*>>>>>DBFIELD_EJERCICIO_CUSTOMDEFAULTVALUE*/
    return ModuleInstance->getEjercicio();
}

} // namespace empresa
} // namespace gong
