/*<<<<<COPYLEFT*/
/** @file produccionreclotedet.cpp Registro de detalles de lotes
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
// COPYLEFT Registro de detalles de lotes
// MEMBER beforeSaveOrDeleteRelated
// Relation Articulo
// TYPE dbRecord produccion::LoteDet
/*>>>>>MODULE_INFO*/

/*<<<<<LOTEDET_INCLUDES*/
#include "produccionreclotedet.h"
/*>>>>>LOTEDET_INCLUDES*/

namespace gong {
namespace produccion {


/*<<<<<LOTEDET_RELATIONS*/
RecArticulo *RecLoteDet::getRecArticulo() const
{
	return static_cast<RecArticulo*>(findRelatedRecord("ARTICULO_ID"));
}

/*>>>>>LOTEDET_RELATIONS*/

bool RecLoteDet::actStocks( dbRecord *master, bool savingdetail ) const
{
	if( master )
		_GONG_DEBUG_PRINT(0, master->getTableName() );
	double cantidad = getValue( "CANTIDAD" ).toDouble();
	getRecArticulo()->actStocks(savingdetail, true /*compra*/, cantidad);
	getRecArticulo()->actStocksComponentes( savingdetail, cantidad );
    return true;
}

/*<<<<<LOTEDET_BEFORESAVEORDELETERELATED*/
void RecLoteDet::beforeSaveOrDeleteRelated(dbRecord *master, bool saving)
{
/*>>>>>LOTEDET_BEFORESAVEORDELETERELATED*/
	actStocks( master, saving );
}


}
}
