/*<<<<<COPYLEFT*/
/** @file facturecproveedora.cpp Registro de proveedoras
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
// COPYLEFT Registro de proveedoras
// RELATION contactos::Contacto
// RELATION Agente
// RELATION pagos::FormaPago
// TYPE dbRecord factu::Proveedora
/*>>>>>MODULE_INFO*/

/*<<<<<PROVEEDORA_INCLUDES*/
#include "facturecproveedora.h"
/*>>>>>PROVEEDORA_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace factu {

/*<<<<<PROVEEDORA_RELATIONS*/
contactos::RecContacto *RecProveedora::getRecContacto() const
{
    return static_cast<contactos::RecContacto*>(findRelatedRecord("PROVEEDORA.CONTACTO_ID"));
}

RecAgente *RecProveedora::getRecAgente() const
{
    return static_cast<RecAgente*>(findRelatedRecord("PROVEEDORA.AGENTE_ID"));
}

pagos::RecFormaPago *RecProveedora::getRecFormaPago() const
{
    return static_cast<pagos::RecFormaPago*>(findRelatedRecord("PROVEEDORA.FORMAPAGO_ID"));
}

/*>>>>>PROVEEDORA_RELATIONS*/

} // namespace factu
} // namespace gong

