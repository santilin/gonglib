/*<<<<<COPYLEFT*/
/** @file empresarectipoiva.cpp Registro de tipos de IVA
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
// COPYLEFT Registro de tipos de IVA
// TYPE dbRecord empresa::TipoIVA
/*>>>>>MODULE_INFO*/

#include "empresamodule.h"
/*<<<<<TIPOIVA_INCLUDES*/
#include "empresarectipoiva.h"
/*>>>>>TIPOIVA_INCLUDES*/


/* Tipos de iva por defecto:
 * General: 18,0%, 4,0%
 * Reducido: 8,0%, 1,0%
 * Superreducido: 4,0%, 0,5%
 */

namespace gong {
namespace empresa {

Money RecTipoIVA::calcIVA(Money cantidad) const
{
    return cantidad * getValue("IVA").toDouble() / 100;
}

Money RecTipoIVA::calcRecargo(Money cantidad) const
{
    return cantidad * getValue("RECARGO").toDouble() / 100;
}

Money RecTipoIVA::calcIVARecargo(Money cantidad) const
{
    return cantidad * (getValue("IVA").toDouble() + getValue("RECARGO").toDouble()) / 100;
}

Money RecTipoIVA::masIVA(Money cantidad) const
{
    return cantidad + (cantidad * getValue("IVA").toDouble() / 100);
}

Money RecTipoIVA::masRecargo(Money cantidad) const
{
    return cantidad + (cantidad * getValue("RECARGO").toDouble() / 100);
}

Money RecTipoIVA::masIVARecargo(Money cantidad) const
{
    return cantidad + (cantidad * (getValue("IVA").toDouble() + getValue("RECARGO").toDouble()) / 100);
}

Money RecTipoIVA::menosIVA(Money cantidad) const
{
    if( getValue("IVA").toDouble() != -100.0 )
        return cantidad / (1 + getValue("IVA").toDouble() / 100);
    else
        return 0.0;
}


Money RecTipoIVA::menosRecargo(Money cantidad) const
{
    if( getValue("RECARGO").toDouble() != -100.0 )
        return cantidad / (1 + getValue("RECARGO").toDouble() / 100);
    else
        return 0.0;
}

Money RecTipoIVA::menosIVARecargo(Money cantidad) const
{
    if( (getValue("IVA").toDouble() + getValue("RECARGO").toDouble() ) != -100.0 )
        return cantidad / (1 + (getValue("IVA").toDouble() + getValue("RECARGO").toDouble() ) / 100);
    else
        return 0.0;
}

} // namespace empresa
} // namespace gong
