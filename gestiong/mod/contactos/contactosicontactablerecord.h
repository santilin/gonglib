#ifndef _CONTACTOS_ICONTACTABLERECORD_H
#define _CONTACTOS_ICONTACTABLERECORD_H

/** @file contactosicontactabelrecord.h Behavior para añadir datos de contacto a un registro
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <gongxtring.h>

namespace gong {

class dbRecord;

namespace contactos {

class IContactableRecord
{
public:
    IContactableRecord( dbRecord *rec )
        : pRecord( rec )
    {}
    Xtring fullAddress( const Xtring &format = Xtring::null ) const;

private:
    dbRecord *pRecord;
};

} // namespace contactos
} // namespace gong


#endif // _CONTACTOS_ICONTACTABLERECORD_H
