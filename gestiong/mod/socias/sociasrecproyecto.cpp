/*<<<<<COPYLEFT*/
/** @file sociasrecproyecto.cpp
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
// TYPE dbRecord socias::Proyecto
/*>>>>>MODULE_INFO*/

/*<<<<<PROYECTO_INCLUDES*/
#include "sociasrecproyecto.h"
/*>>>>>PROYECTO_INCLUDES*/

#include "sociasmodule.h"

namespace gong {
namespace socias {

bool RecProyecto::isActivo() const
{
    return getValue("SOCIASESTADO").toInt() == SociasModule::activo;
}

} // namespace socias
} // namespace gong



