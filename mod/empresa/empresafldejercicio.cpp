/*<<<<<COPYLEFT*/
/** @file empresafldejercicio.cpp dbFieldDefinition que representa el ejercicio en curso
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
// COPYLEFT dbFieldDefinition que representa el ejercicio en curso
// MEMBER isValid
// MEMBER customDefaultValue
// TYPE dbFieldDefinition empresa::Ejercicio NoConstr
/*>>>>>MODULE_INFO*/

#include "empresamodule.h"
/*<<<<<DBFIELD_EJERCICIO_INCLUDES*/
#include "empresafldejercicio.h"
/*>>>>>DBFIELD_EJERCICIO_INCLUDES*/

namespace gong
{
namespace empresa {

/*<<<<<DBFIELD_EJERCICIO_ISVALID*/
bool FldEjercicio::isValid( dbRecord *r, dbFieldValue *value, ValidResult::Context context, ValidResult *integres) const
{
    /*>>>>>DBFIELD_EJERCICIO_ISVALID*/
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
