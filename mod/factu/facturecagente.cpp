/*<<<<<COPYLEFT*/
/** @file facturecagente.cpp Registro de agentes comerciales
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
// COPYLEFT Registro de agentes comerciales
// RELATION contactos::Contacto
// TYPE dbRecord factu::Agente
/*>>>>>MODULE_INFO*/

/*<<<<<AGENTE_INCLUDES*/
#include "facturecagente.h"
/*>>>>>AGENTE_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace factu {

/*<<<<<AGENTE_RELATIONS*/
contactos::RecContacto *RecAgente::getRecContacto() const
{
    return static_cast<contactos::RecContacto*>(findRelatedRecord("AGENTE.CONTACTO_ID"));
}

/*>>>>>AGENTE_RELATIONS*/

} // namespace factu
} // namespace gong

