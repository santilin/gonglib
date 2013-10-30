/*<<<<<COPYLEFT*/
/** @file sociasrecpartidaproyecto.cpp Registro de partidas de proyectos
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
// COPYLEFT Registro de partidas de proyectos
// Relation Proyecto
// TYPE dbRecord socias::PartidaProyecto
/*>>>>>MODULE_INFO*/

/*<<<<<PARTIDAPROYECTO_INCLUDES*/
#include "sociasrecpartidaproyecto.h"
/*>>>>>PARTIDAPROYECTO_INCLUDES*/

namespace gong {
namespace socias {

/*<<<<<PARTIDAPROYECTO_RELATIONS*/
RecProyecto *RecPartidaProyecto::getRecProyecto() const
{
    return static_cast<RecProyecto*>(findRelatedRecord("PARTIDAPROYECTO.PROYECTO_ID"));
}

/*>>>>>PARTIDAPROYECTO_RELATIONS*/

} // namespace socias
} // namespace gong
