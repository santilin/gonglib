/*<<<<<COPYLEFT*/
/** @file factufldivadetallado.cpp dbFieldDefinition que representa el modo de detallar el iva en las facturas, etc.
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
// COPYLEFT dbFieldDefinition que representa el modo de detallar el iva en las facturas, etc.
// INHERIT dbFieldListOfValues<int>
// TYPE dbFieldDefinition factu::IVADetallado static
/*>>>>>MODULE_INFO*/

/*<<<<<DBFIELD_IVADETALLADO_INCLUDES*/
#include "factufldivadetallado.h"
/*>>>>>DBFIELD_IVADETALLADO_INCLUDES*/

namespace gong {
namespace factu {

IntList FldIVADetallado::sValues;
XtringList FldIVADetallado::sCaptions;


} // namespace factu
} // namespace gong
