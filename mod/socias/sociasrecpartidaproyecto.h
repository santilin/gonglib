/*<<<<<COPYLEFT*/
/** @file sociasrecpartidaproyecto.h Registro de partidas de proyectos
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */
/*>>>>>COPYLEFT*/
/*<<<<<PARTIDAPROYECTO_PREAMBLE*/
#ifndef _SOCIAS_SOCIASRECPARTIDAPROYECTO_H
#define _SOCIAS_SOCIASRECPARTIDAPROYECTO_H
/*>>>>>PARTIDAPROYECTO_PREAMBLE*/

/*<<<<<PARTIDAPROYECTO_INCLUDES*/
#include <dbappdbapplication.h>

#include "sociasrecproyecto.h"
/*>>>>>PARTIDAPROYECTO_INCLUDES*/

namespace gong {
namespace socias {

/*<<<<<PARTIDAPROYECTO_CONSTRUCTOR*/
class RecPartidaProyecto: public dbRecord
{
public:
    RecPartidaProyecto(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("PARTIDAPROYECTO"), recid, user)
/*>>>>>PARTIDAPROYECTO_CONSTRUCTOR*/
    {};
    /*<<<<<PARTIDAPROYECTO_RELATIONS*/
    RecProyecto *getRecProyecto() const;
    /*>>>>>PARTIDAPROYECTO_RELATIONS*/

}; // class
/*<<<<<PARTIDAPROYECTO_POSTAMBLE*/
} // namespace socias
} // namespace gong
#endif // SOCIAS_SOCIASRECPARTIDAPROYECTO_H
/*>>>>>PARTIDAPROYECTO_POSTAMBLE*/

