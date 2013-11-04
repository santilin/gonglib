/*<<<<<COPYLEFT*/
/** @file facturecpresupuestoventa.cpp Registro de presupuestos de venta
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
// COPYLEFT Registro de presupuestos de venta
// MEMBER init
// RELATION TipoDoc
// RELATION Cliente
// RELATION Agente
// RELATION PresupuestoVentaDet Detalles
// INTERFACE public IIVADesglosable
// INTERFACE public ITotalizableRecord
// TYPE dbRecord factu::PresupuestoVenta
/*>>>>>MODULE_INFO*/

/*<<<<<PRESUPUESTOVENTA_INCLUDES*/
#include "facturecpresupuestoventa.h"
/*>>>>>PRESUPUESTOVENTA_INCLUDES*/
#include "empresamodule.h"

namespace gong {
namespace factu {

/*<<<<<PRESUPUESTOVENTA_INIT*/
void RecPresupuestoVenta::init()
{
/*>>>>>PRESUPUESTOVENTA_INIT*/
    addStructuralFilter( "PRESUPUESTOVENTA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
}

/*<<<<<PRESUPUESTOVENTA_RELATIONS*/
RecTipoDoc *RecPresupuestoVenta::getRecTipoDoc() const
{
	return static_cast<RecTipoDoc*>(findRelatedRecord("PRESUPUESTOVENTA.TIPODOC_ID"));
}

RecCliente *RecPresupuestoVenta::getRecCliente() const
{
	return static_cast<RecCliente*>(findRelatedRecord("PRESUPUESTOVENTA.CLIENTE_ID"));
}

RecAgente *RecPresupuestoVenta::getRecAgente() const
{
	return static_cast<RecAgente*>(findRelatedRecord("PRESUPUESTOVENTA.AGENTE_ID"));
}

RecPresupuestoVentaDet *RecPresupuestoVenta::getRecPresupuestoVentaDet( int presupuestoventadet ) const
{
	return static_cast<RecPresupuestoVentaDet*>(findRelationByRelatedTable("PRESUPUESTOVENTADET" )->getRelatedRecord( presupuestoventadet));
}

dbRecordList *RecPresupuestoVenta::getListPresupuestoVentaDet() const
{
	return findRelationByRelatedTable( "PRESUPUESTOVENTADET" )->getRelatedRecordList();
}
/*>>>>>PRESUPUESTOVENTA_RELATIONS*/

} // namespace factu
} // namespace gong

