/*<<<<<COPYLEFT*/
/** @file ofertasrecofertaventadet.cpp Registro de detalles de ofertas de venta
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
// COPYLEFT Registro de detalles de ofertas de venta
// MEMBER beforeSaveOrDeleteRelated
// RELATION factu::Articulo
// RELATION empresa::TipoIVA
// TYPE dbRecord ofertas::OfertaVentaDet
/*>>>>>MODULE_INFO*/

/*<<<<<OFERTAVENTADET_INCLUDES*/
#include "ofertasrecofertaventadet.h"
/*>>>>>OFERTAVENTADET_INCLUDES*/
#include "ofertasrecofertaventa.h"

namespace gong {
namespace ofertas {

/*<<<<<OFERTAVENTADET_RELATIONS*/
factu::RecArticulo *RecOfertaVentaDet::getRecArticulo() const
{
	return static_cast<factu::RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

empresa::RecTipoIVA *RecOfertaVentaDet::getRecTipoIVA() const
{
	return static_cast<empresa::RecTipoIVA*>(findRelatedRecord("TIPOIVA_ID"));
}

/*>>>>>OFERTAVENTADET_RELATIONS*/

/*<<<<<OFERTAVENTADET_BEFORESAVEORDELETERELATED*/
void RecOfertaVentaDet::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
/*>>>>>OFERTAVENTADET_BEFORESAVEORDELETERELATED*/
    actStocks( master, saving );
}

bool RecOfertaVentaDet::actStocks( dbRecord *ofertaventa, bool savingdetail ) const
{
    if( static_cast<RecOfertaVenta *>(ofertaventa)->getRecTipoDoc()->getValue("ACTUALIZASTOCKS").toBool() )
        getRecArticulo()->actStocks(savingdetail, false /*venta*/, getValue( "CANTIDAD" ).toDouble());
    return true;
}

} // namespace ofertas
} // namespace gong
