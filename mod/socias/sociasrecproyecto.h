/*<<<<<COPYLEFT*/
/** @file sociasrecproyecto.h 
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
/*<<<<<PROYECTO_PREAMBLE*/
#ifndef _SOCIAS_SOCIASRECPROYECTO_H
#define _SOCIAS_SOCIASRECPROYECTO_H
/*>>>>>PROYECTO_PREAMBLE*/

/*<<<<<PROYECTO_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>PROYECTO_INCLUDES*/

namespace gong {
namespace socias {

/*<<<<<PROYECTO_CONSTRUCTOR*/
class RecProyecto: public dbRecord
{
public:
	RecProyecto(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PROYECTO"), recid, user)
/*>>>>>PROYECTO_CONSTRUCTOR*/
    {}
    bool isActivo() const;
    /*<<<<<PROYECTO_MEMBERS*/

    /*>>>>>PROYECTO_MEMBERS*/

}; // class
/*<<<<<PROYECTO_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASRECPROYECTO_H
/*>>>>>PROYECTO_POSTAMBLE*/

