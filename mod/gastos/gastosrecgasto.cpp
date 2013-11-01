/*<<<<<COPYLEFT*/
/** @file gastosrecgasto.cpp Registro de
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
// COPYLEFT Registro de gastos e ingresos
// MEMBER init
// TYPE dbRecord gastos::Gasto
/*>>>>>MODULE_INFO*/

#include <empresamodule.h>
/*<<<<<GASTO_INCLUDES*/
#include "gastosrecgasto.h"
/*>>>>>GASTO_INCLUDES*/
namespace gong {
namespace gastos {

/*<<<<<GASTO_INIT*/
void RecGasto::init()
{
/*>>>>>GASTO_INIT*/
    addStructuralFilter( "GASTO.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "GASTO.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

} // namespace gastos
} // namespace gong
