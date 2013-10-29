#ifndef _CONTACTOS_RECCONTACTOIMAGEN_H
#define _CONTACTOS_RECCONTACTOIMAGEN_H

/** @file contactosreccontactoimagen.h Registro de imágenes de los contactos
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <dbappdbapplication.h>

namespace gong {
namespace contactos {

class RecContactoImagen: public dbRecord
{
public:
    RecContactoImagen(dbConnection *conn, dbRecordID recid=0, dbUser *user=0)
        : dbRecord(conn, DBAPP->getDatabase()->findTableDefinition("CONTACTOIMAGEN"), recid, user)
    {}
}; // end class

} // namespace contactos
} // namespace gong
#endif // CONTACTOS_RECCONTACTOIMAGEN_H
