/*<<<<<COPYLEFT*/
/** @file empresafldempresaid.cpp dbFieldDefinition que representa el id de la empresa en curso
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


