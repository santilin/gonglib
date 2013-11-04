/*<<<<<COPYLEFT*/
/** @file facturecfamilia.h Registro de familias de artículos
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
/*<<<<<FAMILIA_PREAMBLE*/
#ifndef _FACTU_FACTURECFAMILIA_H
#define _FACTU_FACTURECFAMILIA_H
/*>>>>>FAMILIA_PREAMBLE*/

/*<<<<<FAMILIA_INCLUDES*/
#include <dbappdbapplication.h>

#include "empresarectipoiva.h"
/*>>>>>FAMILIA_INCLUDES*/

namespace gong {
namespace factu {

/*<<<<<FAMILIA_CONSTRUCTOR*/
class RecFamilia: public dbRecord
{
public:
	RecFamilia(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("FAMILIA"), recid, user)
/*>>>>>FAMILIA_CONSTRUCTOR*/
    {};
    /*<<<<<FAMILIA_RELATIONS*/
	empresa::RecTipoIVA *getRecTipoIVA() const;
/*>>>>>FAMILIA_RELATIONS*/
}; // end class

/*<<<<<FAMILIA_POSTAMBLE*/
} // namespace factu
} // namespace gong
#endif // FACTU_FACTURECFAMILIA_H
/*>>>>>FAMILIA_POSTAMBLE*/
