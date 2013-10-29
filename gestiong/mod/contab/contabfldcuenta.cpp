/*<<<<<COPYLEFT*/
/** @file contabfldcuenta.cpp dbFieldDefinition que representa una cuenta
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
