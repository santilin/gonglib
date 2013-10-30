/*<<<<<COPYLEFT*/
/** @file facturecpedidocompra.cpp Registro de pedidos de compra
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
// COPYLEFT Registro de pedidos de compra
// MEMBER init
// RELATION TipoDoc
// RELATION Proveedora
// RELATION Agente
// RELATION PedidoCompraDet Detalles
// INTERFACE public ITotalizableRecord
// TYPE dbRecord factu::PedidoCompra
/*>>>>>MODULE_INFO*/

/*<<<<<PEDIDOCOMPRA_INCLUDES*/
#include "facturecpedidocompra.h"
/*>>>>>PEDIDOCOMPRA_INCLUDES*/
#include <empresamodule.h>

namespace gong {
namespace factu {

/*<<<<<PEDIDOCOMPRA_INIT*/
void RecPedidoCompra::init()
{
    /*>>>>>PEDIDOCOMPRA_INIT*/
    addStructuralFilter( "PEDIDOCOMPRA.EMPRESA_ID=" + getConnection()->toSQL( empresa::ModuleInstance->getEmpresaID() ) );
    addStructuralFilter( "PEDIDOCOMPRA.EJERCICIO=" + getConnection()->toSQL( empresa::ModuleInstance->getEjercicio() ) );
}

/*<<<<<PEDIDOCOMPRA_RELATIONS*/
RecTipoDoc *RecPedidoCompra::getRecTipoDoc() const
{
    return static_cast<RecTipoDoc*>(findRelatedRecord("PEDIDOCOMPRA.TIPODOC_ID"));
}

RecProveedora *RecPedidoCompra::getRecProveedora() const
{
    return static_cast<RecProveedora*>(findRelatedRecord("PEDIDOCOMPRA.PROVEEDORA_ID"));
}

RecAgente *RecPedidoCompra::getRecAgente() const
{
    return static_cast<RecAgente*>(findRelatedRecord("PEDIDOCOMPRA.AGENTE_ID"));
}

RecPedidoCompraDet *RecPedidoCompra::getRecPedidoCompraDet( int pedidocompradet ) const
{
    return static_cast<RecPedidoCompraDet*>(findRelationByRelatedTable("PEDIDOCOMPRADET" )->getRelatedRecord( pedidocompradet));
}

dbRecordList *RecPedidoCompra::getListPedidoCompraDet() const
{
    return findRelationByRelatedTable( "PEDIDOCOMPRADET" )->getRelatedRecordList();
}
/*>>>>>PEDIDOCOMPRA_RELATIONS*/

} // namespace factu
} // namespace gong
