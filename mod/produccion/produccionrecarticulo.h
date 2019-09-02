/*<<<<<COPYLEFT*/
/** @file produccionrecarticulo.h Registro de artículos
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
/*<<<<<ARTICULO_PREAMBLE*/
#ifndef _PRODUCCION_PRODUCCIONRECARTICULO_H
#define _PRODUCCION_PRODUCCIONRECARTICULO_H
/*>>>>>ARTICULO_PREAMBLE*/

/*<<<<<ARTICULO_INCLUDES*/
#include <dbappdbapplication.h>

#include "produccionrecarticulocomponente.h"
#include "produccionreclote.h"
/*>>>>>ARTICULO_INCLUDES*/
#include <facturecarticulo.h>
// {capel} eliminar #include "produccionrecarticulocomponente.h"
// {capel} eliminar #include "produccionreclote.h"

namespace gong {
namespace produccion {

class RecLote;
class RecArticuloComponente;

/*<<<<<ARTICULO_CONSTRUCTOR*/
class RecArticulo: public factu::RecArticulo
{
public:
	RecArticulo(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: factu::RecArticulo(conn, recid, user)
/*>>>>>ARTICULO_CONSTRUCTOR*/
	{};
/*<<<<<ARTICULO_RELATIONS*/
	RecArticuloComponente *getRecArticuloComponente( int narticulocomponente = -1 ) const;
	dbRecordList *getListArticuloComponente() const;
	RecLote *getRecLote() const;
/*>>>>>ARTICULO_RELATIONS*/
	void actStocksComponentes( bool savingdetail, double cantidad );
	bool hasComponente( dbRecordID other_art_id ) const;
}; // end class

/*<<<<<ARTICULO_POSTAMBLE*/
} // namespace produccion
} // namespace gong
#endif // PRODUCCION_PRODUCCIONRECARTICULO_H
/*>>>>>ARTICULO_POSTAMBLE*/

