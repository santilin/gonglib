/*<<<<<COPYLEFT*/
/** @file produccionrecarticulocomponente.cpp Registro de productos de artículos
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de productos de artículos
// RELATION Articulo
// TYPE dbRecord produccion::ArticuloComponente
/*>>>>>MODULE_INFO*/

/*<<<<<ARTICULOCOMPONENTE_INCLUDES*/
#include "produccionrecarticulocomponente.h"
/*>>>>>ARTICULOCOMPONENTE_INCLUDES*/
#include "produccionrecarticulo.h"


namespace gong {
namespace produccion {

/*<<<<<ARTICULOCOMPONENTE_RELATIONS*/
RecArticulo *RecArticuloComponente::getRecArticulo() const
{
	return static_cast<RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

/*>>>>>ARTICULOCOMPONENTE_RELATIONS*/

} // namespace produccion
} // namespace gong

