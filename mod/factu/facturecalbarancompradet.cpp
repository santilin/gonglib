/*<<<<<COPYLEFT*/
/** @file facturecalbarancompradet.cpp Registro de detalles de albaranes de compra
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
// COPYLEFT Registro de detalles de albaranes de compra
// MEMBER beforeSaveOrDeleteRelated
// RELATION Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord factu::AlbaranCompraDet
/*>>>>>MODULE_INFO*/

/*<<<<<ALBARANCOMPRADET_INCLUDES*/
#include "facturecalbarancompradet.h"
/*>>>>>ALBARANCOMPRADET_INCLUDES*/
#include "facturecalbarancompra.h"

namespace gong {
namespace factu {

/*<<<<<ALBARANCOMPRADET_RELATIONS*/
RecArticulo *RecAlbaranCompraDet::getRecArticulo() const
{
	return static_cast<RecArticulo*>(findRelatedRecord("ALBARANCOMPRADET.ARTICULO_ID"));
}

empresa::RecTipoIVA *RecAlbaranCompraDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("ALBARANCOMPRADET.TIPOIVA_ID"));
}

/*>>>>>ALBARANCOMPRADET_RELATIONS*/

/*<<<<<ALBARANCOMPRADET_BEFORESAVEORDELETERELATED*/
void RecAlbaranCompraDet::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
/*>>>>>ALBARANCOMPRADET_BEFORESAVEORDELETERELATED*/
    actStocks( master, saving );
}

bool RecAlbaranCompraDet::actStocks( dbRecord *albarancompra, bool savingdetail ) const
{
    if( static_cast<RecAlbaranCompra *>(albarancompra)->getRecTipoDoc()->getValue("ACTUALIZASTOCKS").toBool() )
        getRecArticulo()->actStocks(savingdetail, true /*compra*/, getValue( "CANTIDAD" ).toDouble());
    return true;
}

} // namespace factu
} // namespace gong
