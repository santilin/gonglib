/*<<<<<COPYLEFT*/
/** @file empresarecempresa.cpp
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
// RELATION contactos::Contacto
// RELATION Moneda
// MEMBER save
// TYPE dbRecord empresa::Empresa
/*>>>>>MODULE_INFO*/

#include <empresamodule.h>
/*<<<<<EMPRESA_INCLUDES*/
#include "empresarecempresa.h"
/*>>>>>EMPRESA_INCLUDES*/


namespace gong {
namespace empresa {

/*<<<<<EMPRESA_RELATIONS*/
contactos::RecContacto *RecEmpresa::getRecContacto() const
{
	return static_cast<contactos::RecContacto*>(findRelatedRecord("EMPRESA.CONTACTO_ID"));
}

RecMoneda *RecEmpresa::getRecMoneda() const
{
	return static_cast<RecMoneda*>(findRelatedRecord("EMPRESA.MONEDA_ID"));
}

/*>>>>>EMPRESA_RELATIONS*/

/*<<<<<EMPRESA_SAVE*/
bool RecEmpresa::save(bool saverelated) throw( dbError )
{
/*>>>>>EMPRESA_SAVE*/
    // Si se modifica la empresa actual, actualizar la referencia en este módulo a la empresa actual
    bool ret = dbRecord::save(saverelated);
    if( ret && getRecordID() == ModuleInstance->getRecEmpresa()->getRecordID() )
        ModuleInstance->getRecEmpresa()->copyRecord( this );
    return ret;
}

} // namespace empresa
} // namespace gong

