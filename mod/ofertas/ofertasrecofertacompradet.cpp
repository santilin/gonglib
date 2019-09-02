/*<<<<<COPYLEFT*/
/** @file ofertasrecofertacompradet.cpp Registro de detalles ofertas de compra
 * GongLib project. (L) Santiago Capel Torres, 2003-2011
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/*>>>>>COPYLEFT*/
/*<<<<<MODULE_INFO*/
// COPYLEFT Registro de detalles ofertas de compra
// MEMBER beforeSaveOrDeleteRelated
// RELATION factu::Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord ofertas::OfertaCompraDet
/*>>>>>MODULE_INFO*/

/*<<<<<OFERTACOMPRADET_INCLUDES*/
#include "ofertasrecofertacompradet.h"
/*>>>>>OFERTACOMPRADET_INCLUDES*/
#include "ofertasrecofertacompra.h"

namespace gong {
namespace ofertas {

/*<<<<<OFERTACOMPRADET_RELATIONS*/
factu::RecArticulo *RecOfertaCompraDet::getRecArticulo() const
{
	return static_cast<factu::RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

empresa::RecTipoIVA *RecOfertaCompraDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>OFERTACOMPRADET_RELATIONS*/
/*<<<<<OFERTACOMPRADET_BEFORESAVEORDELETERELATED*/
void RecOfertaCompraDet::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
/*>>>>>OFERTACOMPRADET_BEFORESAVEORDELETERELATED*/
    actStocks( master, saving );
}

bool RecOfertaCompraDet::actStocks( dbRecord *albarancompra, bool savingdetail ) const
{
    if( static_cast<RecOfertaCompra *>(albarancompra)->getRecTipoDoc()->getValue("ACTUALIZASTOCKS").toBool() )
        getRecArticulo()->actStocks(savingdetail, true /*compra*/, getValue( "CANTIDAD" ).toDouble());
    return true;
}

} // namespace ofertas
} // namespace gong
