/*<<<<<COPYLEFT*/
/** @file sociasrectiposocia.cpp Registro de tipos de socia
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
// COPYLEFT Registro de tipos de socia
// MEMBER isValid
// TYPE dbRecord socias::TipoSocia
/*>>>>>MODULE_INFO*/

/*<<<<<TIPOSOCIA_INCLUDES*/
#include "sociasrectiposocia.h"
/*>>>>>TIPOSOCIA_INCLUDES*/

namespace gong {
namespace socias {

/*<<<<<TIPOSOCIA_ISVALID*/
bool RecTipoSocia::isValid(ValidResult::Context context, ValidResult *result )
{
    bool ret = dbRecord::isValid(context, result);
    /*>>>>>TIPOSOCIA_ISVALID*/
    return ret;
}

} // namespace socias
} // namespace gong

