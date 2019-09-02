/*<<<<<COPYLEFT*/
/** @file produccionrecarticulocomponente.h Registro de productos de artículos
 * Proyecto GestiONG. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<ARTICULOCOMPONENTE_PREAMBLE*/
#ifndef _PRODUCCION_PRODUCCIONRECARTICULOCOMPONENTE_H
#define _PRODUCCION_PRODUCCIONRECARTICULOCOMPONENTE_H
/*>>>>>ARTICULOCOMPONENTE_PREAMBLE*/

/*<<<<<ARTICULOCOMPONENTE_INCLUDES*/
#include <dbappdbapplication.h>

#include "produccionrecarticulo.h"
/*>>>>>ARTICULOCOMPONENTE_INCLUDES*/
// {capel} Eliminar el #include produccionrecarticulo.h

namespace gong {
namespace produccion {

class RecArticulo;

/*<<<<<ARTICULOCOMPONENTE_CONSTRUCTOR*/
class RecArticuloComponente: public dbRecord
{
public:
	RecArticuloComponente(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("ARTICULOCOMPONENTE"), recid, user)
/*>>>>>ARTICULOCOMPONENTE_CONSTRUCTOR*/
	{}
/*<<<<<ARTICULOCOMPONENTE_RELATIONS*/
	RecArticulo *getRecArticulo() const;
/*>>>>>ARTICULOCOMPONENTE_RELATIONS*/
}; // end class

/*<<<<<ARTICULOCOMPONENTE_POSTAMBLE*/
} // namespace produccion
} // namespace gong
#endif // PRODUCCION_PRODUCCIONRECARTICULOCOMPONENTE_H
/*>>>>>ARTICULOCOMPONENTE_POSTAMBLE*/

