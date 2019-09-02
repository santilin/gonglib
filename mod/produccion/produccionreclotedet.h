/*<<<<<COPYLEFT*/
/** @file produccionreclotedet.h Registro de detalles de lotes
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
/*<<<<<LOTEDET_PREAMBLE*/
#ifndef _PRODUCCION_PRODUCCIONRECLOTEDET_H
#define _PRODUCCION_PRODUCCIONRECLOTEDET_H
/*>>>>>LOTEDET_PREAMBLE*/

/*<<<<<LOTEDET_INCLUDES*/
#include <dbappdbapplication.h>

#include "produccionrecarticulo.h"
/*>>>>>LOTEDET_INCLUDES*/

namespace gong {
namespace produccion {

/*<<<<<LOTEDET_CONSTRUCTOR*/
class RecLoteDet: public dbRecord
{
public:
	RecLoteDet(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("LOTEDET"), recid, user)
/*>>>>>LOTEDET_CONSTRUCTOR*/
	{};
/*<<<<<LOTEDET_RELATIONS*/
	RecArticulo *getRecArticulo() const;
/*>>>>>LOTEDET_RELATIONS*/
protected:
/*<<<<<LOTEDET_MEMBERS*/
	virtual void beforeSaveOrDeleteRelated(dbRecord *master, bool saving); // from dbRecord
/*>>>>>LOTEDET_MEMBERS*/
	bool actStocks( dbRecord *master, bool savingdetail ) const;
}; // end class

/*<<<<<LOTEDET_POSTAMBLE*/
} // namespace produccion
} // namespace gong
#endif // PRODUCCION_PRODUCCIONRECLOTEDET_H
/*>>>>>LOTEDET_POSTAMBLE*/
