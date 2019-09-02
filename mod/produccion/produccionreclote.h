/*<<<<<COPYLEFT*/
/** @file produccionreclote.h Registro de lotes
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
/*<<<<<LOTE_PREAMBLE*/
#ifndef _PRODUCCION_PRODUCCIONRECLOTE_H
#define _PRODUCCION_PRODUCCIONRECLOTE_H
/*>>>>>LOTE_PREAMBLE*/

/*<<<<<LOTE_INCLUDES*/
#include <dbappdbapplication.h>

#include "produccionreclotedet.h"
/*>>>>>LOTE_INCLUDES*/

namespace gong {
namespace produccion {

/*<<<<<LOTE_CONSTRUCTOR*/
class RecLote: public dbRecord
{
public:
	RecLote(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("LOTE"), recid, user)
/*>>>>>LOTE_CONSTRUCTOR*/
	{};
/*<<<<<LOTE_RELATIONS*/
	RecLoteDet *getRecLoteDet( int nlotedet = -1 ) const;
	dbRecordList *getListLoteDet() const;
/*>>>>>LOTE_RELATIONS*/
}; // end class

/*<<<<<LOTE_POSTAMBLE*/
} // namespace produccion
} // namespace gong
#endif // PRODUCCION_PRODUCCIONRECLOTE_H
/*>>>>>LOTE_POSTAMBLE*/
