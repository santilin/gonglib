/*<<<<<COPYLEFT*/
/** @file produccionrecarticulo.cpp Registro de artículos
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
// COPYLEFT Registro de artículos
// INHERIT factu::Articulo
// RELATION ArticuloComponente Detalles
// RELATION Lote
// TYPE dbRecord produccion::Articulo
/*>>>>>MODULE_INFO*/

#include <gongdbrecordlist.h>
/*<<<<<ARTICULO_INCLUDES*/
#include "produccionrecarticulo.h"
/*>>>>>ARTICULO_INCLUDES*/
#include "produccionrecarticulocomponente.h"
#include "produccionreclote.h"

namespace gong {
namespace produccion {

/*<<<<<ARTICULO_RELATIONS*/
RecArticuloComponente *RecArticulo::getRecArticuloComponente( int articulocomponente ) const
{
	return static_cast<RecArticuloComponente*>(findRelationByRelatedTable("ARTICULOCOMPONENTE" )->getRelatedRecord( articulocomponente));
}

dbRecordList *RecArticulo::getListArticuloComponente() const
{
	return findRelationByRelatedTable( "ARTICULOCOMPONENTE" )->getRelatedRecordList();
}
RecLote *RecArticulo::getRecLote() const
{
	return static_cast<RecLote*>(findRelatedRecord("ARTICULO.LOTE_ID"));
}

/*>>>>>ARTICULO_RELATIONS*/

void RecArticulo::actStocksComponentes( bool savingdetail, double cantidad )
{
	for( dbRecordList::const_iterator compit = getListArticuloComponente()->begin();
		compit != getListArticuloComponente()->end(); ++ compit ) {
		RecArticuloComponente *componente = static_cast<RecArticuloComponente *>(*compit);
		produccion::RecArticulo *articulo = componente->getRecArticulo();
		articulo->actStocks( savingdetail, false /* como si se vendieran */,
							 cantidad * componente->getValue( "CANTIDAD" ).toDouble() );
		articulo->actStocksComponentes( savingdetail,
			cantidad * componente->getValue( "CANTIDAD" ).toDouble() );
	}
}

/**
 * @brief Comprueba recursivamente si un articulo_id es un componente de este artículo
 *
 * @param other_art_id ...
 * @return bool
 **/
bool RecArticulo::hasComponente(dbRecordID other_art_id) const
{
	for( dbRecordList::const_iterator compit = getListArticuloComponente()->begin();
		compit != getListArticuloComponente()->end(); ++ compit ) {
		RecArticuloComponente *componente = static_cast<RecArticuloComponente *>(*compit);
		if( componente->getValue( "ARTICULO_ID" ).toUInt() == other_art_id ) {
			return true;
		} else return componente->getRecArticulo()->hasComponente( other_art_id );
	}
	return false;
}


} // namespace produccion
} // namespace gong
