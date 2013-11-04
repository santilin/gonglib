/*<<<<<COPYLEFT*/
/** @file facturecfamilia.cpp Registro de familias de artículos
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
// COPYLEFT Registro de familias de artículos
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::Familia
/*>>>>>MODULE_INFO*/

/*<<<<<FAMILIA_INCLUDES*/
#include "facturecfamilia.h"
/*>>>>>FAMILIA_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace factu {

/*<<<<<FAMILIA_RELATIONS*/
empresa::RecTipoIVA *RecFamilia::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("FAMILIA.TIPOIVA_ID"));
}

/*>>>>>FAMILIA_RELATIONS*/

} // namespace factu
} // namespace gong
