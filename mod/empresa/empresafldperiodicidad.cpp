/*<<<<<COPYLEFT*/
/** @file empresafldperiodicidad.cpp dbFieldDefinition que representa el estado del pago de un cobro o un pago
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
// COPYLEFT dbFieldDefinition que representa el estado del pago de un cobro o un pago
// INHERIT dbFieldListOfValues<int>
// TYPE dbFieldDefinition empresa::Periodicidad static
/*>>>>>MODULE_INFO*/

#include <dbappdbapplication.h>
/*<<<<<DBFIELD_PERIODICIDAD_INCLUDES*/
#include "empresafldperiodicidad.h"
/*>>>>>DBFIELD_PERIODICIDAD_INCLUDES*/

namespace gong {
namespace empresa {

List<int> FldPeriodicidad::mValues;
XtringList FldPeriodicidad::mCaptions;

}
}
