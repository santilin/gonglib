/*<<<<<COPYLEFT*/
/** @file factureccliente.cpp Registro de clientes
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
// COPYLEFT Registro de clientes
// RELATION contactos::Contacto
// RELATION pagos::FormaPago
// RELATION Agente
// INTERFACE public contactos::IContactableRecord
// TYPE dbRecord factu::Cliente
/*>>>>>MODULE_INFO*/

/*<<<<<CLIENTE_INCLUDES*/
#include "factureccliente.h"
/*>>>>>CLIENTE_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace factu {

/*<<<<<CLIENTE_RELATIONS*/
contactos::RecContacto *RecCliente::getRecContacto() const
{
    return static_cast<contactos::RecContacto*>(findRelatedRecord("CLIENTE.CONTACTO_ID"));
}

pagos::RecFormaPago *RecCliente::getRecFormaPago() const
{
    return static_cast<pagos::RecFormaPago*>(findRelatedRecord("CLIENTE.FORMAPAGO_ID"));
}

RecAgente *RecCliente::getRecAgente() const
{
    return static_cast<RecAgente*>(findRelatedRecord("CLIENTE.AGENTE_ID"));
}

/*>>>>>CLIENTE_RELATIONS*/

} // namespace factu
} // namespace gong

