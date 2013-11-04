/*<<<<<COPYLEFT*/
/** @file pagosrecremesacobro.cpp Registro de remesas de recibos de proyectos
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
// COPYLEFT Registro de remesas de recibos de proyectos
// RELATION empresa::Proyecto
// MEMBER save
// TYPE dbRecord pagos::RemesaCobro
/*>>>>>MODULE_INFO*/

/*<<<<<REMESACOBRO_INCLUDES*/
#include "pagosrecremesacobro.h"
/*>>>>>REMESACOBRO_INCLUDES*/

namespace gong {
namespace pagos {

void RecRemesaCobro::actTotales()
{
    Variant nrecibos = 0, importe = Money(0.0), resto = Money(0.0);
    if( getRecordID() )
        getConnection()->selectValues( "SELECT COUNT(*), SUM(IMPORTE), SUM(RESTO) FROM COBRO"
                                       " WHERE REMESACOBRO_ID=" + getConnection()->toSQL( getRecordID() ), &nrecibos, &importe, &resto );
    setValue( "NRECIBOS", nrecibos );
    setValue( "IMPORTE", importe );
    setValue( "RESTO", importe );
}

/*<<<<<REMESACOBRO_SAVE*/
bool RecRemesaCobro::save(bool saverelated) throw( dbError )
{
/*>>>>>REMESACOBRO_SAVE*/
    actTotales();
    return dbRecord::save(saverelated);
}

/*<<<<<REMESACOBRO_RELATIONS*/
empresa::RecProyecto *RecRemesaCobro::getRecProyecto() const
{
	return static_cast<empresa::RecProyecto*>(findRelatedRecord("REMESACOBRO.PROYECTO_ID"));
}

/*>>>>>REMESACOBRO_RELATIONS*/

} // namespace pagos
} // namespace gong
