/*<<<<<COPYLEFT*/
/** @file empresarecproyecto.h Registro de proyectos
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
#ifndef _EMPRESA_EMPRESARECPROYECTO_H
#define _EMPRESA_EMPRESARECPROYECTO_H
/*>>>>>PROYECTO_PREAMBLE*/

/*<<<<<PROYECTO_INCLUDES*/
#include <dbappdbapplication.h>

/*>>>>>PROYECTO_INCLUDES*/

namespace gong {
namespace empresa {

/*<<<<<PROYECTO_CONSTRUCTOR*/
class RecProyecto: public dbRecord
{
public:
	RecProyecto(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
		: dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PROYECTO"), recid, user)
/*>>>>>PROYECTO_CONSTRUCTOR*/
    {};
}; // end class

/*<<<<<PROYECTO_POSTAMBLE*/
} // namespace empresa
} // namespace gong
#endif // EMPRESA_EMPRESARECPROYECTO_H
/*>>>>>PROYECTO_POSTAMBLE*/

