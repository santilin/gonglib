/*<<<<<COPYLEFT*/
/** @file sociasrectiposocia.h Registro de tipos de socia
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
/*<<<<<TIPOSOCIA_PREAMBLE*/
#ifndef _SOCIAS_SOCIASRECTIPOSOCIA_H
#define _SOCIAS_SOCIASRECTIPOSOCIA_H
/*>>>>>TIPOSOCIA_PREAMBLE*/

/*<<<<<TIPOSOCIA_INCLUDES*/
#include <dbappdbapplication.h>
#include <gongdbvalidresult.h>

/*>>>>>TIPOSOCIA_INCLUDES*/

namespace gong {
namespace socias {

/*<<<<<TIPOSOCIA_CONSTRUCTOR*/
class RecTipoSocia: public dbRecord
{
public:
	RecTipoSocia(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("TIPOSOCIA"), recid, user)
/*>>>>>TIPOSOCIA_CONSTRUCTOR*/
    {}
    /*<<<<<TIPOSOCIA_MEMBERS*/
	virtual bool isValid(ValidResult::Context context, ValidResult *result=0); // from dbRecord
/*>>>>>TIPOSOCIA_MEMBERS*/

}; // class
/*<<<<<TIPOSOCIA_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASRECTIPOSOCIA_H
/*>>>>>TIPOSOCIA_POSTAMBLE*/

