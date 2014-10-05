/*<<<<<CONCEPTOTESORERIA_PREAMBLE*/
#ifndef _TESORERIA_TESORERIARECCONCEPTOTESORERIA_H
#define _TESORERIA_TESORERIARECCONCEPTOTESORERIA_H
/*>>>>>CONCEPTOTESORERIA_PREAMBLE*/

/*<<<<<COPYLEFT*/
/** @file tesoreriarecconceptotesoreria.h Registro de
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
#include <dbappdbapplication.h>

/*<<<<<CONCEPTOTESORERIA_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>CONCEPTOTESORERIA_INCLUDES*/

namespace gong {
namespace tesoreria {


/*<<<<<CONCEPTOTESORERIA_CONSTRUCTOR*/
class RecConceptoTesoreria: public dbRecord
{
public:
	RecConceptoTesoreria(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("CONCEPTOTESORERIA"), recid, user)
/*>>>>>CONCEPTOTESORERIA_CONSTRUCTOR*/
    {};
}; // end class

/*<<<<<CONCEPTOTESORERIA_POSTAMBLE*/
} // namespace tesoreria
} // namespace gong
#endif // TESORERIA_TESORERIARECCONCEPTOTESORERIA_H
/*>>>>>CONCEPTOTESORERIA_POSTAMBLE*/

